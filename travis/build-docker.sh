#!/bin/bash

set -e -x 
docker build --file Dockerfile.manylinux --tag alphacep/kaldi-manylinux:latest .
docker run --rm -v /home/codex/Documents/Projects/trash/vosk-api:/io alphacep/kaldi-manylinux /io/travis/build-wheels.sh
