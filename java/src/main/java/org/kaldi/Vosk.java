package org.kaldi;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.annotation.Platform;

@Platform(include = {"vosk_api.h"}, link = {"vosk"},
        preload = {"quadmath@.0", "gfortran@.5"})
public class Vosk {

    static {
        Loader.load();
    }

    private static native void vosk_set_log_level(int level);

    public static void setLogLevel(int level) {
        vosk_set_log_level(level);
    }
}
