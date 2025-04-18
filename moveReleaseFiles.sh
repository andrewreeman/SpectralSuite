#!/bin/bash

rm -r -f test-release
mkdir -p test-release/MacOSX/AU
mkdir -p test-release/MacOSX/VST3

find cmake-build-release/*/*_artefacts/Release/VST3/*.vst3 -maxdepth 0 -exec cp -r {} test-release/MacOSX/VST3/ \;
find cmake-build-release/*/*_artefacts/Release/AU/*.component -maxdepth 0 -exec cp -r {} test-release/MacOSX/AU/ \;
