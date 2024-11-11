<<<<<<< HEAD
cmake_minimum_required(VERSION 3.29)

add_executable(dro
    dro-stm.cpp

    cortexm/cortexm3.s
    cortexm/clockstarter.cpp
    cortexm/nvic.cpp
    cortexm/systick.cpp

    cortexm/fifo.cpp
    cortexm/wtf.cpp

    cortexm/stm32/stm32.cpp
    cortexm/stm32/gpio.cpp
    cortexm/stm32/afio.cpp
    cortexm/stm32/exti.cpp
    cortexm/stm32/clocksf1.cpp
    cortexm/stm32/dmaf1.cpp
    cortexm/stm32/flashf1.cpp
    cortexm/stm32/timer.cpp
    cortexm/stm32/uart.cpp

    ezcpp/bitbasher.cpp
    ezcpp/sharedtimer.cpp
    ezcpp/quadraturecounter.cpp
    ezcpp/stopwatch.cpp
    cortexm/stm32/bluepill.cpp
    cortexm/stm32/bluepill.h
)

add_compile_options(-fpermissive)

target_compile_options(dro PUBLIC -fpermissive)

include_directories(
    .
    cortexm
    cortexm/stm32
    ezcpp
)
=======

#slowly splitting the cmake from this file which will hopefully be sharable someday, and CMakeLists.txt.

#kill warning: qt sets this when it launches cmake, but we don't produce QT stuff so we got a warning.
unset(QT_QMAKE_EXECUTABLE)

## ST device macro
#SET (DEVICE "STM32F103xx")
##todo: set up link to irqs file, invoke builder as needed.
#ADD_DEFINITIONS(-D${DEVICE})

# Magic settings. Without it CMake tries to run test programs on the host platform, which fails of course.
SET (CMAKE_SYSTEM_NAME Generic)

# specify the cross compiler
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(AS arm-none-eabi-as)
set(AR arm-none-eabi-ar)
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)


##################### CHIP Configuration ###################################
set(TARGET arm-arm-none-eabi)
set(CMAKE_SYSTEM_PROCESSOR Cortex-M3)
set(CHIP STM32F103xB)
set(CPU_FAMILY STM32F10X_MD)

##SET(COMMON_FLAGS "-mcpu=${mcpu} $${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0 ${linkerFlags}")
##SET(CMAKE_CXX_FLAGS_INIT "$${COMMON_FLAGS} -std=c++14")
##SET(CMAKE_C_FLAGS_INIT "$${COMMON_FLAGS} -std=c11")

SET(CMAKE_C_FLAGS "-mcpu=cortex-m3 -std=c11 -fdata-sections -ffunction-sections -Wall" CACHE INTERNAL "c compiler flags")
# -Wno-unknown-pragmas added to hide spew from clang pragmas that hide clang spew. phew!
SET(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -std=c++14 -fdata-sections -ffunction-sections -Wall -fno-rtti -fno-exceptions -Wno-unknown-pragmas  -MD " CACHE INTERNAL "cxx compiler flags")

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
SET (CMAKE_EXE_LINKER_FLAGS "-L ${PROJECT_SOURCE_DIR} -T ${LINKER_SCRIPT} -nostartfiles  -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--print-memory-usage,-Map,${PROJECT_NAME}.map " CACHE INTERNAL "exe link flags")

#add_custom_command(TARGET $${PROJECT_NAME}.elf POST_BUILD COMMAND $${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:$${PROJECT_NAME}.elf> $${BIN_FILE} COMMENT "Building $${BIN_FILE}")
>>>>>>> 571452d480ea6f428c16a016e629dc3722a7fc7d
