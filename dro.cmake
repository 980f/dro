
#slowly splitting the cmake from this file which will hopefully be sharable someday, and CMakeLists.txt.

#kill warning: qt sets this when it launches cmake, but we don't produce QT stuff so we got a warning.
unset(QT_QMAKE_EXECUTABLE)

## ST device macro
#SET (DEVICE "STM32F103xx")
##todo: set up link to irqs file, invoke builder as needed.
#ADD_DEFINITIONS(-D${DEVICE})

# Magic settings. Without it CMake tries to run test programs on the host platform, which fails of course.
SET (CMAKE_SYSTEM_NAME Generic)
SET (CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_FLAGS "-mcpu=cortex-m3 -std=c11 -fdata-sections -ffunction-sections -Wall" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -std=c++14 -fno-rtti -fno-exceptions -Wall -fdata-sections -ffunction-sections -MD -Wall" CACHE INTERNAL "cxx compiler flags")

#todo: see if we can drop these, they are in the GCC tree.
INCLUDE_DIRECTORIES(${SUPPORT_FILES})
#needed for soft floating point:
LINK_DIRECTORIES(${SUPPORT_FILES})


# project include paths .
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES("cortexm")
INCLUDE_DIRECTORIES("cortexm/stm32")
INCLUDE_DIRECTORIES("ezcpp")

SET (LINKER_SCRIPT "${PROJECT_NAME}.ld")

#todo: try to eliminate -specs=nosys.specs, we are suppressing routines that are referenced therein but will never execute.
#todo: try to find a variable to provide /d/work/pro, the official one craters the compiler detect.
SET (CMAKE_EXE_LINKER_FLAGS "-L /d/work/dro -T ${LINKER_SCRIPT} -nostartfiles -specs=nosys.specs -Wl,--gc-sections,-Map,${PROJECT_NAME}.map " CACHE INTERNAL "exe link flags")
