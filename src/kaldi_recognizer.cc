// Copyright 2019-2020 Alpha Cephei Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "kaldi_recognizer.h"
#include "json.h"
#include "fstext/fstext-utils.h"
#include "lat/sausages.h"


using namespace fst;
using namespace kaldi::nnet3;

KaldiRecognizer::KaldiRecognizer(float sample_frequency, SpkModel *spk_model) : spk_model_(spk_model), sample_frequency_(sample_frequency) {

    spk_model->Ref();

    spk_feature_ = new OnlineMfcc(spk_model_->spkvector_mfcc_opts);
}

KaldiRecognizer::~KaldiRecognizer() {
    if (spk_model_)
         delete spk_feature_;
}

bool KaldiRecognizer::AcceptWaveform(const char *data, int len)
{
    Vector<BaseFloat> wave;
    wave.Resize(len / 2, kUndefined);
    for (int i = 0; i < len / 2; i++)
        wave(i) = *(((short *)data) + i);
    return AcceptWaveform(wave);
}

bool KaldiRecognizer::AcceptWaveform(const short *sdata, int len)
{
    Vector<BaseFloat> wave;
    wave.Resize(len, kUndefined);
    for (int i = 0; i < len; i++)
        wave(i) = sdata[i];
    return AcceptWaveform(wave);
}

bool KaldiRecognizer::AcceptWaveform(const float *fdata, int len)
{
    Vector<BaseFloat> wave;
    wave.Resize(len, kUndefined);
    for (int i = 0; i < len; i++)
        wave(i) = fdata[i];
    return AcceptWaveform(wave);
}

bool KaldiRecognizer::AcceptWaveform(Vector<BaseFloat> &wdata)
{
    state_ = RECOGNIZER_RUNNING;

    spk_feature_->AcceptWaveform(sample_frequency_, wdata);

    return true;
}

// Computes an xvector from a chunk of speech features.
static void RunNnetComputation(const MatrixBase<BaseFloat> &features,
    const nnet3::Nnet &nnet, nnet3::CachingOptimizingCompiler *compiler,
    Vector<BaseFloat> *xvector)
{
    nnet3::ComputationRequest request;
    request.need_model_derivative = false;
    request.store_component_stats = false;
    request.inputs.push_back(
    nnet3::IoSpecification("input", 0, features.NumRows()));
    nnet3::IoSpecification output_spec;
    output_spec.name = "output";
    output_spec.has_deriv = false;
    output_spec.indexes.resize(1);
    request.outputs.resize(1);
    request.outputs[0].Swap(&output_spec);
    shared_ptr<const nnet3::NnetComputation> computation = compiler->Compile(request);
    nnet3::Nnet *nnet_to_update = nullptr;  // we're not doing any update.
    nnet3::NnetComputer computer(nnet3::NnetComputeOptions(), *computation,
                    nnet, nnet_to_update);
    CuMatrix<BaseFloat> input_feats_cu(features);
    computer.AcceptInput("input", &input_feats_cu);
    computer.Run();
    CuMatrix<BaseFloat> cu_output;
    computer.GetOutputDestructive("output", &cu_output);
    xvector->Resize(cu_output.NumCols());
    xvector->CopyFromVec(cu_output.Row(0));
}

#define MIN_SPK_FEATS 50

bool KaldiRecognizer::GetSpkVector(Vector<BaseFloat> &out_xvector, int *num_spk_frames)
{
    vector<int32> nonsilence_frames;

    int num_frames = spk_feature_->NumFramesReady() - frame_offset_ * 3;
    Matrix<BaseFloat> mfcc(num_frames, spk_feature_->Dim());

    // Not very efficient, would be nice to have faster search
    int num_nonsilence_frames = 0;
    Vector<BaseFloat> feat(spk_feature_->Dim());

    for (int i = 0; i < num_frames; ++i) {
       if (std::find(nonsilence_frames.begin(),
                     nonsilence_frames.end(), i / 3) == nonsilence_frames.end()) {
           continue;
       }

       spk_feature_->GetFrame(i + frame_offset_ * 3, &feat);
       mfcc.CopyRowFromVec(feat, num_nonsilence_frames);
       num_nonsilence_frames++;
    }

    *num_spk_frames = num_nonsilence_frames;

    // Don't extract vector if not enough data
    if (num_nonsilence_frames < MIN_SPK_FEATS) {
        return false;
    }

    mfcc.Resize(num_nonsilence_frames, spk_feature_->Dim(), kCopyData);

    SlidingWindowCmnOptions cmvn_opts;
    cmvn_opts.center = true;
    cmvn_opts.cmn_window = 300;
    Matrix<BaseFloat> features(mfcc.NumRows(), mfcc.NumCols(), kUndefined);
    SlidingWindowCmn(cmvn_opts, mfcc, &features);

    nnet3::NnetSimpleComputationOptions opts;
    nnet3::CachingOptimizingCompilerOptions compiler_config;
    nnet3::CachingOptimizingCompiler compiler(spk_model_->speaker_nnet, opts.optimize_config, compiler_config);

    Vector<BaseFloat> xvector;
    RunNnetComputation(features, spk_model_->speaker_nnet, &compiler, &xvector);

    // Whiten the vector with global mean and transform and normalize mean
    xvector.AddVec(-1.0, spk_model_->mean);

    out_xvector.Resize(spk_model_->transform.NumRows(), kSetZero);
    out_xvector.AddMatVec(1.0, spk_model_->transform, kNoTrans, xvector, 0.0);

    BaseFloat norm = out_xvector.Norm(2.0);
    BaseFloat ratio = norm / sqrt(out_xvector.Dim()); // how much larger it is
                                                  // than it would be, in
                                                  // expectation, if normally
    out_xvector.Scale(1.0 / ratio);

    return true;
}


const char *KaldiRecognizer::MbrResult()
{
    json::JSON obj;
    stringstream text;

    if (spk_model_) {
        Vector<BaseFloat> xvector;
        int num_spk_frames;
        if (GetSpkVector(xvector, &num_spk_frames)) {
            for (int i = 0; i < xvector.Dim(); i++) {
                obj["spk"].append(xvector(i));
            }
            obj["spk_frames"] = num_spk_frames;
        }
    }

    return StoreReturn(obj.dump());
}

const char* KaldiRecognizer::FinalResult()
{
    if (state_ != RECOGNIZER_RUNNING) {
        return StoreEmptyReturn();
    }

    MbrResult();

    // Free some memory while we are finalized, next
    // iteration will reinitialize them anyway
    delete spk_feature_;

    spk_feature_ = nullptr;

    return last_result_.c_str();
}
const char *KaldiRecognizer::StoreEmptyReturn()
{
    return StoreReturn("{\"text\": \"\"}");
}
// Store result in recognizer and return as const string
const char *KaldiRecognizer::StoreReturn(const string &res)
{
    last_result_ = res;
    return last_result_.c_str();
}
