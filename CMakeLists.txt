cmake_minimum_required(VERSION 3.29)

set(CortexmVendor stm32)
set(VendorPartname f103)
set(VendorVariant c8)
set(BoardName bluepill)
#processor selection:


#cmake bitched about 'project' being in an include file.
project(dro-${CortexmVendor}
  DESCRIPTION "digital read out, remote report to a phone"
  LANGUAGES CXX
)

#project files are out of line in case we need to make l452.cmake and friends include them so that the postamble will work better.
include(dro.cmake)

#it would be nice if we could define the things herein sooner, but I haven't figured out how to forward reference the source list.
include(cortexm/postamble.cmake)
