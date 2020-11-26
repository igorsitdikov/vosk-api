
1. install msys2 

    ```
        https://www.msys2.org/
        https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20200903.exe
    ```
   
1. update packages

    ```
        pacman -Syuu
    ```

1. restart terminal

1. install dependencies

    ```
        pacman -S base-devel git mingw-w64-x86_64-toolchain make mingw-w64-x86_64-cmake mingw-w64-x86_64-dlfcn mingw-w64-i686-qt-creator mingw-w64-x86_64-qt-creator
        
        OPTIONAL:
        pacman -S mingw-w64-i686-qt-creator mingw-w64-x86_64-qt-creator
        pacman -S mingw-w64-x86_64-openblas
    ```

1. clone kaldi

    ```
        git clone --depth 1 --branch android-mix https://github.com/alphacep/kaldi.git
    ```

1. compile openfst
    ```
        cd ~/kaldi/tools
   
        git clone --depth 1 --branch win32 https://github.com/alphacep/openfst.git
   
        cd openfst
   
        autoreconf -i
   
        ./configure --enable-static --enable-shared --enable-far --enable-ngram-fsts --enable-lookahead-fsts --with-pic --disable-bin
        
        make 
   ```

1. change path in `~/kaldi/src/makefiles/mingw.mk`
  
    ```
        -L/home/shmyrev/android/kaldi-win/mingw64_dll
    ```

1. download OpenBLAS
    
    ```
    cd ~/kaldi/tools
    curl -L -O http://sourceforge.net/projects/openblas/files/v0.2.14/OpenBLAS-v0.2.14-Win64-int32.zip
    curl -L -O http://sourceforge.net/projects/openblas/files/v0.2.14/mingw64_dll.zip
    unzip OpenBLAS-v0.2.14-Win64-int32.zip
    unzip mingw64_dll.zip
   ```

1. compile kaldi

    ```
        mkdir -p lib/fst && \
        mv src/include/ include/ && \
        mv src/lib/.libs/* lib/ && \
        mv src/extensions/far/.libs/* lib/ && \
        mv src/extensions/lookahead/.libs/* lib/fst/ && \
        mv src/extensions/ngram/.libs/* lib/ 
   
        ./configure --mingw=/mingw --static --openblas-root=../tools/OpenBLAS --fst-root=../tools/openfst/ --use-cuda=no && \
        
        make
    ```
1. optional
    ```
        export PATH=$PATH:"/c/msys64/apache-maven-3.6.3/bin"
        export PATH=$PATH:"/c/msys64/gradle-6.7.1/bin"
        export PATH=$PATH:"/c/msys64/jdk-11/bin"
    ```
