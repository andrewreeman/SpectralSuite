cmake_minimum_required(VERSION 3.22)
project(SpectralSuite VERSION 2.1.12)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.2
    VERSION 8.0.2
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

include(shared/shared.cmake)
set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)

add_subdirectory(Morph)

