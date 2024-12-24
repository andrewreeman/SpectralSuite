# TODO: see if we can use a smaller image
FROM ubuntu:latest

RUN apt update

RUN apt install -y libasound2-dev libjack-jackd2-dev \
        ladspa-sdk \
        libcurl4-openssl-dev  \
        libfreetype6-dev \
        libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
        libwebkit2gtk-4.1-dev \
        libglu1-mesa-dev mesa-common-dev \
        cmake

RUN apt install -y make gcc g++
RUN apt install -y git

RUN export CC=gcc && export CXX=gcc

RUN mkdir /ss && cd /ss

COPY . /ss

RUN mkdir /ss/build-unix && cd /ss/build-unix && cmake ..
RUN cd /ss/build-unix && cmake build . && make
RUN cd /ss/build-unix && mkdir artefacts \
    && cp -r "BinScrambler/BinScrambler_artefacts/VST3/Bin Scrambler.vst3/" artefacts/ \
    && cp -r "FrequencyMagnet/FreqMagnet_artefacts/VST3/Frequency Magnet.vst3/" artefacts/ \
    && cp -r "FrequencyShift/FreqShift_artefacts/VST3/Frequency Shifter.vst3/" artefacts/ \
    && cp -r "Morph/Morph_artefacts/VST3/Morph.vst3/" artefacts/ \
    && cp -r "PhaseLock/PhaseLock_artefacts/VST3/Phase Lock.vst3/" artefacts/ \
    && cp -r "SinusoidalShapedFilter/Ssf_artefacts/VST3/Sinusoidal Shaped Filter.vst3/" artefacts/ \
    && cp -r "SpectralGate/SpecGate_artefacts/VST3/Spectral Gate.vst3/" artefacts/ \
