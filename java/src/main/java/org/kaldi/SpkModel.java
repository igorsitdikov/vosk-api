package org.kaldi;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.Platform;

@Platform(include = {"spk_model.h"}, link = {"vosk"},
    preload = {"gcc_s@.1", "quadmath@.0", "gfortran@.4", "stdc++@.6", "m@.6", "c@.6"})
public class SpkModel extends Pointer {

    static {
        Loader.load();
    }

    public SpkModel(String spk_path) {
        allocate(spk_path);
    }

    private native void allocate(String spk_path);
}
