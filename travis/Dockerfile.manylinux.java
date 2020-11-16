FROM quay.io/pypa/manylinux2010_x86_64

LABEL description="A docker image for building portable Python linux binary wheels and Kaldi"
LABEL maintainer="contact@alphacephei.com"

RUN yum -y update && yum -y install \
    wget \
    openssl-devel \
    pcre-devel \
    automake \
    autoconf \
    libtool \
    && yum clean all

RUN cd /opt \
    && wget -O swig-4.0.1.tar.gz https://sourceforge.net/projects/swig/files/swig/swig-4.0.1/swig-4.0.1.tar.gz/download \
    && tar xf swig-4.0.1.tar.gz \
    && cd swig-4.0.1 \
    && ./configure --prefix=/usr && make -j 10 && make install \
    && cd .. \
    && rm -rf swig-4.0.1.tar.gz swig-4.0.1

RUN cd /opt \
    && git clone -b lookahead --single-branch https://github.com/alphacep/kaldi \
    && cd kaldi/tools \
    && git clone -b v0.3.7 --single-branch https://github.com/xianyi/OpenBLAS \
    && make PREFIX=$(pwd)/OpenBLAS/install DYNAMIC_ARCH=1 USE_LOCKING=1 USE_THREAD=0 -C OpenBLAS all install \
    && git clone https://github.com/alphacep/openfst openfst \
    && cd openfst \
    && autoreconf -i \
    && ./configure --prefix=/opt/kaldi/tools/openfst --enable-static --enable-shared --enable-far --enable-ngram-fsts --enable-lookahead-fsts --with-pic --disable-bin \
    && make -j 10 && make install \
    && cd /opt/kaldi/src \
    && ./configure --mathlib=OPENBLAS --shared --use-cuda=no \
    && make -j 10 online2 lm rnnlm \
    && find /opt/kaldi -name "*.o" -exec rm {} \;

ARG MAVEN_VERSION=3.6.3

        #RUN curl -O https://download.java.net/java/GA/jdk11/13/GPL/openjdk-11.0.1_linux-x64_bin.tar.gz && \
        #    tar zxvf openjdk-11.0.1_linux-x64_bin.tar.gz && \
        #    mv jdk-11.0.1 /usr/local/
        #
        ## create new
        #ENV JAVA_HOME /usr/local/jdk-11.0.1
        #ENV PATH $JAVA_HOME/bin:$PATH

RUN yum update && yum install -y \
java-1.8.0-openjdk java-1.8.0-openjdk-devel

# Changing user to root to install maven
USER root

# Install required tools
# which: otherwise 'mvn version' prints '/usr/share/maven/bin/mvn: line 93: which: command not found'
RUN yum update -y && \
  yum install -y which && \
  yum clean all

# Maven
RUN curl -fsSL https://archive.apache.org/dist/maven/maven-3/$MAVEN_VERSION/binaries/apache-maven-$MAVEN_VERSION-bin.tar.gz | tar xzf - -C /usr/share \
  && mv /usr/share/apache-maven-$MAVEN_VERSION /usr/share/maven \
  && ln -s /usr/share/maven/bin/mvn /usr/bin/mvn

ENV MAVEN_VERSION=${MAVEN_VERSION}
ENV M2_HOME /usr/share/maven
ENV maven.home $M2_HOME
ENV M2 $M2_HOME/bin
ENV PATH $M2:$PATH

RUN mkdir /recipies

WORKDIR /recipies/