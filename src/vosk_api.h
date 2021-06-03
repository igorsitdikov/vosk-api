// Copyright 2020-2021 Alpha Cephei Inc.
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

/* This header contains the C API for Vosk speech recognition system */

#ifndef VOSK_API_H
#define VOSK_API_H

#ifdef __cplusplus
extern "C" {
#endif

/** Speaker model is the same as model but contains the data
 *  for speaker identification. */
typedef struct VoskSpkModel VoskSpkModel;


/** Recognizer object is the main object which processes data.
 *  Each recognizer usually runs in own thread and takes audio as input.
 *  Once audio is processed recognizer returns JSON object as a string
 *  which represent decoded information - words, confidences, times, n-best lists,
 *  speaker information and so on */
typedef struct VoskRecognizer VoskRecognizer;



/** Loads speaker model data from the file and returns the model object
 *
 * @param model_path: the path of the model on the filesystem
 * @returns model object */
VoskSpkModel *vosk_spk_model_new(const char *model_path);


/** Releases the model memory
 *
 *  The model object is reference-counted so if some recognizer
 *  depends on this model, model might still stay alive. When
 *  last recognizer is released, model will be released too. */
void vosk_spk_model_free(VoskSpkModel *model);



/** Creates the recognizer object with speaker recognition
 *
 *  With the speaker recognition mode the recognizer not just recognize
 *  text but also return speaker vectors one can use for speaker identification
 *
 *  @param sample_rate The sample rate of the audio you going to feed into the recognizer
 *  @param spk_model speaker model for speaker identification
 *  @returns recognizer object */
VoskRecognizer *vosk_recognizer_new_spk(float sample_rate, VoskSpkModel *spk_model);

/** Accept voice data
 *
 *  accept and process new chunk of voice data
 *
 *  @param data - audio data in PCM 16-bit mono format
 *  @param length - length of the audio data
 *  @returns true if silence is occured and you can retrieve a new utterance with result method */
int vosk_recognizer_accept_waveform(VoskRecognizer *recognizer, const char *data, int length);


/** Same as above but the version with the short data for language bindings where you have
 *  audio as array of shorts */
int vosk_recognizer_accept_waveform_s(VoskRecognizer *recognizer, const short *data, int length);


/** Same as above but the version with the float data for language bindings where you have
 *  audio as array of floats */
int vosk_recognizer_accept_waveform_f(VoskRecognizer *recognizer, const float *data, int length);

/** Returns speech recognition result. Same as result, but doesn't wait for silence
 *  You usually call it in the end of the stream to get final bits of audio. It
 *  flushes the feature pipeline, so all remaining audio chunks got processed.
 *
 *  @returns speech result in JSON format.
 */
const char *vosk_recognizer_final_result(VoskRecognizer *recognizer);


/** Releases recognizer object
 *
 *  Underlying model is also unreferenced and if needed released */
void vosk_recognizer_free(VoskRecognizer *recognizer);

/** Set log level for Kaldi messages
 *
 *  @param log_level the level
 *     0 - default value to print info and error messages but no debug
 *     less than 0 - don't print info messages
 *     greather than 0 - more verbose mode
 */
void vosk_set_log_level(int log_level);


#ifdef __cplusplus
}
#endif

#endif /* VOSK_API_H */
