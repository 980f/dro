cmake_minimum_required(VERSION 3.29)

set (CortexmVendor stm32)
set (VendorPartname f103)
#processor selection:

# project language options, there will be defaults.
set(CMAKE_CXX_STANDARD 17)

#cmake bitched about 'project' being in an include file.
project(dro-${CortexmVendor}
    DESCRIPTION "digital read out, remote report to a phone"
    LANGUAGES CXX
)

#the part related stuff uses project settings so must follow it
include(cortexm/${CortexmVendor}/${VendorPartname}.cmake)
#the above includes a shared file for compiler setup.

message(STATUS "******************************************************************************")
INCLUDE_DIRECTORIES("cortexm/${CortexmVendor}")

#project files are out of line in case we need to make l452.cmake and friends include them so that the postamble will work better.
include(dro.cmake)

#it would be nice if we could define the things herein sooner, but I haven't figured out how to forward reference the source list.
include(cortexm/postamble.cmake)

#had to add this to get cortexm3.s to be processed:
enable_language(ASM)

set(SOURCES
  "dro-${CortexmVendor}.cpp"
  ${CORTEXUSAGES}
  ${${CortexmVendor}_VENDOR_USAGES}
  ${EZUSAGES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
