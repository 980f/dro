#things somewhat independent of which board we use, so long as it uses some sort of cortexm part.
# this must be called by your top level CMakeLists.txt
# it must be followed in there by postamble.cmake where all the action is triggered.
cmake_minimum_required(VERSION 3.29)

#kill warning: qt sets this when it launches cmake, but we don't produce QT stuff so we got a warning.
unset(QT_QMAKE_EXECUTABLE)

#the part related stuff uses project settings so must follow it
include(cortexm/${CortexmVendor}/${VendorPartname}.cmake)
#the above includes a shared file for compiler setup.

message(STATUS "******************************************************************************")

# project language option
set(CMAKE_CXX_STANDARD 17)

include_directories("cortexm/${CortexmVendor}")
set(CMAKE_C_FLAGS "-mcpu=${gcccpu}  -fdata-sections -ffunction-sections -Wall" CACHE INTERNAL "c compiler flags")
# -Wno-unknown-pragmas added to hide spew from clang pragmas that hide clang spew. phew!
set(CMAKE_CXX_FLAGS "-mcpu=${gcccpu} -fdata-sections -ffunction-sections -Wall -fno-rtti -fno-exceptions -Wno-unknown-pragmas  -MD " CACHE INTERNAL "cxx compiler flags")

#todo: see if we can drop these, they are in the GCC tree.
include_directories(${SUPPORT_FILES})
#needed for soft floating point:
link_directories(${SUPPORT_FILES})

# project include paths .
include_directories(".")

include_directories("cortexm")
set(CORTEXUSAGES ${CORTEXUSAGES}
  cortexm/bitbanding.cpp
  cortexm/clockstarter.cpp
  cortexm/fifo.cpp
  cortexm/core-atomic.cpp
  cortexm/systick.cpp
  cortexm/wtf.cpp
  cortexm/nvic.cpp
  cortexm/core_cmfunc.cpp
  cortexm/${gcccpu}.cpp
  cortexm/stackfault.cpp
)

include_directories("ezcpp")
set(EZUSAGES
  ezcpp/sharedtimer.cpp
  ezcpp/bitbasher.cpp
  ezcpp/quadraturecounter.cpp
  ezcpp/quadraturecounter.h
)

if (${CortexmVendor} STREQUAL "stm32")
  set(${CortexmVendor}_VENDOR_USAGES
    cortexm/stm32/${BoardName}.cpp
    cortexm/stm32/stm32.cpp
    cortexm/stm32/gpio.cpp
    cortexm/stm32/exti.cpp
    cortexm/stm32/${CLOCK_MODULE}.cpp
    cortexm/stm32/afio.cpp
    cortexm/stm32/uart.cpp
    cortexm/stm32/dmatransfer.cpp
    cortexm/stm32/dmatransfer.h
  )
endif ()

#for each processor family we will have to add a section here.

message(STATUS ${CortexmVendor} ${VendorPartname} " on " ${BoardName})
message(STATUS "${CORTEXUSAGES}")
message(STATUS "${EZUSAGES}")
message(STATUS "${${CortexmVendor}_VENDOR_USAGES}")

set(SOURCES
  ${CORTEXUSAGES}
  ${EZUSAGES}
  ${${CortexmVendor}_VENDOR_USAGES}
)

set(LINKER_SCRIPT "${PROJECT_NAME}.ld")

#todo: try to eliminate -specs=nosys.specs, we are suppressing routines that are referenced therein but will never execute.
set(CMAKE_EXE_LINKER_FLAGS "-L ${PROJECT_SOURCE_DIR} -T ${LINKER_SCRIPT} -nostartfiles  -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--print-memory-usage,-Map,${PROJECT_NAME}.map " CACHE INTERNAL "exe link flags")

