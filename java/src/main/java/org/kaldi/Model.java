package org.kaldi;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.Platform;

@Platform(include = {"model.h"}, link = {"vosk"},
    preload = {"gcc_s@.1", "quadmath@.0", "gfortran@.4", "stdc++@.6", "m@.6", "c@.6"})
public class Model extends Pointer {

    static {
        Loader.load();
    }

    public Model(String model_path) {
        allocate(model_path);
    }

    private native void allocate(String model_path);
}
