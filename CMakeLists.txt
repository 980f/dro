cmake_minimum_required(VERSION 3.29)

#processor selection:
include(cortexm/stm32/f103.cmake)
#the above includes a shared file for compiler setup.

# project language options, there will be defaults.
set(CMAKE_CXX_STANDARD 17)

#cmake bitched about 'project' being in an include file.
project(dro-stm
    DESCRIPTION "digital read out, remote report to a phone"
    LANGUAGES CXX
)

message(STATUS "******************************************************************************")

#project files are out of line in case we need to make l452.cmake and friends include them so that the postamble will work better.
include(dro.cmake)

#it would be nice if we could define the things herein sooner, but I haven't figured out how to forward reference the source list.
include(cortexm/postamble.cmake)

#had to add this to get cortexm3.s to be processed:
enable_language(ASM)

set(STMUSAGES
  "cortexm/stm32/bluepill.cpp"
  "cortexm/stm32/${GPIO_MODULE}.cpp"
  "cortexm/stm32/exti.cpp"
  "cortexm/stm32/${CLOCK_MODULE}.cpp"
  "cortexm/stm32/afio.cpp"
  "cortexm/stm32/stm32.cpp"
)

set(CORTEXUSAGES
  "cortexm/clockstarter.cpp"
  "cortexm/fifo.cpp"
  "cortexm/systick.cpp"
  "cortexm/wtf.cpp"
  "cortexm/cstartup.cpp"
  "cortexm/nvic.cpp"
  "cortexm/cortexm3.s"
)

set(EZUSAGES
  "ezcpp/sharedtimer.cpp"
)

set(SOURCES
  "dro-stm.cpp"
  ${CORTEXUSAGES}
  ${STMUSAGES}
  ${EZUSAGES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
