// Copyright 2019 Alpha Cephei Inc.
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

#ifndef VOSK_KALDI_RECOGNIZER_H
#define VOSK_KALDI_RECOGNIZER_H

#include "base/kaldi-common.h"
#include "util/common-utils.h"
#include "fstext/fstext-lib.h"
#include "fstext/fstext-utils.h"
#include "decoder/lattice-faster-decoder.h"
#include "feat/feature-mfcc.h"
#include "lat/kaldi-lattice.h"
#include "lat/word-align-lattice.h"
#include "lat/compose-lattice-pruned.h"
#include "nnet3/am-nnet-simple.h"
#include "nnet3/nnet-am-decodable-simple.h"
#include "nnet3/nnet-utils.h"

#include "spk_model.h"

using namespace kaldi;

enum KaldiRecognizerState {
    RECOGNIZER_INITIALIZED,
    RECOGNIZER_RUNNING,
    RECOGNIZER_ENDPOINT,
    RECOGNIZER_FINALIZED
};

class KaldiRecognizer {
    public:
        KaldiRecognizer(float sample_frequency, SpkModel *spk_model);
        ~KaldiRecognizer();
        bool AcceptWaveform(const char *data, int len);
        bool AcceptWaveform(const short *sdata, int len);
        bool AcceptWaveform(const float *fdata, int len);
        const char* FinalResult();

    private:
        bool AcceptWaveform(Vector<BaseFloat> &wdata);
        bool GetSpkVector(Vector<BaseFloat> &out_xvector, int *frames);
        const char *StoreEmptyReturn();
        const char *StoreReturn(const string &res);
        const char *MbrResult();


        // Speaker identification
        SpkModel *spk_model_ = nullptr;
        OnlineBaseFeature *spk_feature_ = nullptr;

        // Rescoring
        fst::ArcMapFst<fst::StdArc, kaldi::LatticeArc, fst::StdToLatticeMapper<kaldi::BaseFloat> > *lm_fst_ = nullptr;

        int max_alternatives_ = 0; // Disable alternatives by default

        float sample_frequency_;
        int32 frame_offset_;

        int64 samples_processed_;
        int64 samples_round_start_;

        KaldiRecognizerState state_;
        string last_result_;
};

#endif /* VOSK_KALDI_RECOGNIZER_H */
