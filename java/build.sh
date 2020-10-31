#!/bin/bash

mvn clean install || exit 1;
mvn install:install-file -Dfile="target/vosk-1.0.0-SNAPSHOT.jar" -DgroupId=org.kaldi -DartifactId=vosk -Dversion=1.0.0-SNAPSHOT -Dpackaging=jar -DgeneratePom=true