#-------------------------------------------------
#
# Project created by QtCreator 2020-11-26T10:54:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = asr
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mywidgetvoicetotext.cpp \
        $$PWD/../IdeaProjects/vosk-api1/src/kaldi_recognizer.cc \
        $$PWD/../IdeaProjects/vosk-api1/src/language_model.cc \
        $$PWD/../IdeaProjects/vosk-api1/src/model.cc \
        $$PWD/../IdeaProjects/vosk-api1/src/spk_model.cc \
        $$PWD/../IdeaProjects/vosk-api1/src/vosk_api.cc

HEADERS += \
    mywidgetvoicetotext.h

INCLUDEPATH += "$$PWD/../kaldi/src"
INCLUDEPATH += "$$PWD/../kaldi/tools/openfst/include"

LIBS += \
        $$PWD/../kaldi/src/online2/kaldi-online2.a \
        $$PWD/../kaldi/src/decoder/kaldi-decoder.a \
        $$PWD/../kaldi/src/ivector/kaldi-ivector.a \
        $$PWD/../kaldi/src/gmm/kaldi-gmm.a \
        $$PWD/../kaldi/src/nnet3/kaldi-nnet3.a \
        $$PWD/../kaldi/src/tree/kaldi-tree.a \
        $$PWD/../kaldi/src/feat/kaldi-feat.a \
        $$PWD/../kaldi/src/lat/kaldi-lat.a \
        $$PWD/../kaldi/src/lm/kaldi-lm.a \
        $$PWD/../kaldi/src/hmm/kaldi-hmm.a \
        $$PWD/../kaldi/src/transform/kaldi-transform.a \
        $$PWD/../kaldi/src/cudamatrix/kaldi-cudamatrix.a \
        $$PWD/../kaldi/src/matrix/kaldi-matrix.a \
        $$PWD/../kaldi/src/fstext/kaldi-fstext.a \
        $$PWD/../kaldi/src/util/kaldi-util.a \
        $$PWD/../kaldi/src/base/kaldi-base.a \
        $$PWD/../kaldi/tools/OpenBLAS/libopenblas.a \
        $$PWD/../kaldi/tools/openfst/lib/libfst.a \
        $$PWD/../kaldi/tools/openfst/lib/libfstngram.a \
        -lgfortran -lpthread \
        -L/usr/local/cuda/lib64 -lcudart -lcurand -lcublas -lcusparse -lcusolver -ldl

INCLUDEPATH += $$PWD/../IdeaProjects/vosk-api1/src

LIBS += $$PWD/../IdeaProjects/vosk-api1/java/libvosk.so


