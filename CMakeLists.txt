cmake_minimum_required(VERSION 3.29)

#processor selection:
include(cortexm/stm32/f103.cmake)
#the above includes a shared file for compiler setup.

# project language options, there will be defaults.
set(CMAKE_CXX_STANDARD 17)

project(dro
    DESCRIPTION "digital read out, remote report to a phone"
    LANGUAGES CXX
)

#project files are out of line in case we need to make l452.cmake and friends include them so that the postamble will work better.
include(dro.cmake)

#it would be nice if we could define the things herein sooner, but I haven't figured out how to forward reference the source list.
include(cortexm/postamble.cmake)

