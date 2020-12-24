#!/bin/bash

set -e
set -x

docker build --file Dockerfile.manylinux --tag alphacep/kaldi-manylinux:latest .
docker run --rm -e PLAT=manylinux2010_x86_64 -v /home/sitdikov/IdeaProjects/vosk-api1/:/recipies alphacep/kaldi-manylinux /io/travis/build-wheels.sh

docker run -it --rm -v /home/sitdikov/IdeaProjects/vosk-api-last/:/recipies alphacep/kaldi-manylinux bash