cmake_minimum_required(VERSION 3.29)

#slowly splitting the cmake from this file which will hopefully be sharable someday, and CMakeLists.txt.

#kill warning: qt sets this when it launches cmake, but we don't produce QT stuff so we got a warning.
unset(QT_QMAKE_EXECUTABLE)

## ST device macro
#SET (DEVICE "STM32F103xx")
##todo: set up link to irqs file, invoke builder as needed.
#ADD_DEFINITIONS(-D${DEVICE})

SET(CMAKE_C_FLAGS "-mcpu=cortex-m3 -std=c11 -fdata-sections -ffunction-sections -Wall" CACHE INTERNAL "c compiler flags")
# -Wno-unknown-pragmas added to hide spew from clang pragmas that hide clang spew. phew!
SET(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -fdata-sections -ffunction-sections -Wall -fno-rtti -fno-exceptions -Wno-unknown-pragmas  -MD " CACHE INTERNAL "cxx compiler flags")

#todo: see if we can drop these, they are in the GCC tree.
INCLUDE_DIRECTORIES(${SUPPORT_FILES})
#needed for soft floating point:
LINK_DIRECTORIES(${SUPPORT_FILES})


# project include paths .
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES("cortexm")

INCLUDE_DIRECTORIES("ezcpp")

SET (LINKER_SCRIPT "${PROJECT_NAME}.ld")

#todo: try to eliminate -specs=nosys.specs, we are suppressing routines that are referenced therein but will never execute.
#todo: try to find a variable to provide /d/work/pro, the official one craters the compiler detect.
SET (CMAKE_EXE_LINKER_FLAGS "-L ${PROJECT_SOURCE_DIR} -T ${LINKER_SCRIPT} -nostartfiles  -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--print-memory-usage,-Map,${PROJECT_NAME}.map " CACHE INTERNAL "exe link flags")

#add_custom_command(TARGET $${PROJECT_NAME}.elf POST_BUILD COMMAND $${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:$${PROJECT_NAME}.elf> $${BIN_FILE} COMMENT "Building $${BIN_FILE}")