package org.kaldi;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.Cast;
import org.bytedeco.javacpp.annotation.Platform;

@Platform(include = {"kaldi_recognizer.h"}, link = {"vosk"},
    preload = {"gcc_s@.1", "quadmath@.0", "gfortran@.4", "stdc++@.6", "m@.6", "c@.6"})
public class KaldiRecognizer extends Pointer {

    static {
        Loader.load();
    }

    public KaldiRecognizer(Model model, float samplerate) {
        allocate(model, samplerate);
    }

    public KaldiRecognizer(Model model, SpkModel spkModel, float samplerate) {
        allocate(model, spkModel, samplerate);
    }

    public KaldiRecognizer(Model model, float samplerate, String grammar) {
        allocate(model, samplerate, grammar);
    }

    private native void allocate(Model model, float samplerate);

    private native void allocate(Model model, SpkModel spkModel, float samplerate);

    private native void allocate(Model model, float samplerate, String grammar);

    public native boolean AcceptWaveform(@Cast("const short*") short[] data, int len);

    public native boolean AcceptWaveform(@Cast("const float*") float[] data, int len);

    public native boolean AcceptWaveform(@Cast("const char*") byte[] data, int len);

    public boolean AcceptWaveform(byte[] data) {
        return AcceptWaveform(data, data.length);
    }

    public boolean AcceptWaveform(short[] data) {
        return AcceptWaveform(data, data.length);
    }

    public boolean AcceptWaveform(float[] data) {
        return AcceptWaveform(data, data.length);
    }

    public native String FinalResult();

    public native String PartialResult();

    public native String Result();
}
