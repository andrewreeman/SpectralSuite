FROM ubuntu:24.04

RUN apt update

RUN apt install -y libasound2-dev libjack-jackd2-dev \
        ladspa-sdk \
        libcurl4-openssl-dev  \
        libfreetype6-dev \
        libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
        libwebkit2gtk-4.1-dev \
        libglu1-mesa-dev mesa-common-dev \
        cmake make gcc g++ git

# TODO: investigate why using ENV for the vars results in ' undefined reference
# to symbol 'exp@@GLIBC_2.29' errors during make. So currently we use 'RUN
# export' instead
#ENV CC=gcc
#ENV CXX=gcc
RUN export CC=gcc && export CXX=gcc
WORKDIR /ss

#RUN mkdir /ss && cd /ss

# COPY . /ss

# TODO: populate this list automatically when new plugin created 
COPY shared shared
COPY CMakeLists.txt .
COPY cmake cmake
COPY BinScrambler BinScrambler
COPY FrequencyMagnet FrequencyMagnet
COPY FrequencyShift FrequencyShift
COPY Morph Morph
COPY PhaseLock PhaseLock
COPY SinusoidalShapedFilter SinusoidalShapedFilter
COPY SpectralGate SpectralGate

RUN mkdir build-linux && cd build-linux && cmake ..
RUN cd build-linux && cmake build . && make
RUN cd build-linux && mkdir artefacts \
    && cp -r "BinScrambler/BinScrambler_artefacts/VST3/Bin Scrambler.vst3/" artefacts/ \
    && cp -r "FrequencyMagnet/FreqMagnet_artefacts/VST3/Frequency Magnet.vst3/" artefacts/ \
    && cp -r "FrequencyShift/FreqShift_artefacts/VST3/Frequency Shifter.vst3/" artefacts/ \
    && cp -r "Morph/Morph_artefacts/VST3/Morph.vst3/" artefacts/ \
    && cp -r "PhaseLock/PhaseLock_artefacts/VST3/Phase Lock.vst3/" artefacts/ \
    && cp -r "SinusoidalShapedFilter/Ssf_artefacts/VST3/Sinusoidal Shaped Filter.vst3/" artefacts/ \
    && cp -r "SpectralGate/SpecGate_artefacts/VST3/Spectral Gate.vst3/" artefacts/ \
