
# ST device macro
SET (DEVICE "STM32F103xx")
#todo: set up link to irqs file, invoke builder as needed.

# Magic settings. Without it CMake tries to run test programs on the host platform, which fails of course.
SET (CMAKE_SYSTEM_NAME Generic)
SET (CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_FLAGS "-mcpu=cortex-m3 -std=c11 -fdata-sections -ffunction-sections -Wall" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -std=c++14 -fno-rtti -fno-exceptions -Wall -fdata-sections -ffunction-sections -MD -Wall" CACHE INTERNAL "cxx compiler flags")


#todo: see if we can drop these, they are in the GCC tree.
INCLUDE_DIRECTORIES(${SUPPORT_FILES})
LINK_DIRECTORIES(${SUPPORT_FILES})

ADD_DEFINITIONS(-D${DEVICE})

# project include paths .
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES("cortexm")
INCLUDE_DIRECTORIES("cortexm/stm32")
INCLUDE_DIRECTORIES("ezcpp")

SET (LINKER_SCRIPT "dro-stm.ld")

#target_link_libraries(-lgcc)

#todo: try to eliminate -specs=nosys.specs, we are suppressing routines that are referenced therein but will never execute.
#todo: try to find a varialbe to provide /d/work/pro, the official one craters the compiler detect.
SET (CMAKE_EXE_LINKER_FLAGS "-L /d/work/dro -T ${LINKER_SCRIPT} -nostartfiles  -specs=nosys.specs -Wl,--gc-sections,-Map,../dro/dro.map " CACHE INTERNAL "exe link flags")
