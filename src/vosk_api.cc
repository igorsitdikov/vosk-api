// Copyright 2020 Alpha Cephei Inc.
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

#include "vosk_api.h"
#include "kaldi_recognizer.h"
#include "spk_model.h"

#include <string.h>

using namespace kaldi;


VoskSpkModel *vosk_spk_model_new(const char *model_path)
{
    return (VoskSpkModel *)new SpkModel(model_path);
}

void vosk_spk_model_free(VoskSpkModel *model)
{
    ((SpkModel *)model)->Unref();
}

VoskRecognizer *vosk_recognizer_new_spk(float sample_rate, VoskSpkModel *spk_model)
{
    return (VoskRecognizer *)new KaldiRecognizer(sample_rate, (SpkModel *)spk_model);
}

void vosk_recognizer_accept_waveform(VoskRecognizer *recognizer, const char *data, int length)
{
    ((KaldiRecognizer *)(recognizer))->AcceptWaveform(data, length);
}

void vosk_recognizer_accept_waveform_s(VoskRecognizer *recognizer, const short *data, int length)
{
    ((KaldiRecognizer *)(recognizer))->AcceptWaveform(data, length);
}

void vosk_recognizer_accept_waveform_f(VoskRecognizer *recognizer, const float *data, int length)
{
    ((KaldiRecognizer *)(recognizer))->AcceptWaveform(data, length);
}

const char *vosk_recognizer_final_result(VoskRecognizer *recognizer)
{
    return ((KaldiRecognizer *)recognizer)->FinalResult();
}

void vosk_recognizer_free(VoskRecognizer *recognizer)
{
    delete (KaldiRecognizer *)(recognizer);
}

void vosk_set_log_level(int log_level)
{
    SetVerboseLevel(log_level);
}
