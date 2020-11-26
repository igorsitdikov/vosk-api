QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mywidgetvoicetotext.cpp\
    $$PWD/../vosk-api/src/kaldi_recognizer.cc \
    $$PWD/../vosk-api/src/language_model.cc \
    $$PWD/../vosk-api/src/model.cc \
    $$PWD/../vosk-api/src/spk_model.cc \
    $$PWD/../vosk-api/src/vosk_api.cc

HEADERS += \
    mywidgetvoicetotext.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
#CCFLAG += -g -O2 -DFST_NO_DYNAMIC_LINKING
#LIBS +=  -lm  -ldl -lpthread

#CCFLAG +=-g -O2 -DPIC -fPIC -Wno-unused-function
INCLUDEPATH += "$$PWD/../kaldi/src"
INCLUDEPATH += "$$PWD/../kaldi/tools/openfst/include"
#INCLUDEPATH += "$$PWD/../kaldi/tools/mingw_dll"

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
        -lgfortran -lpthread -ldl

INCLUDEPATH += $$PWD/../vosk-api/src
DEPENDPATH += $$PWD/../vosk-api/src

win32-g++: PRE_TARGETDEPS += $$PWD/../vosk-api/c/libvosk.a
