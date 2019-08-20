cmake_minimum_required(VERSION 3.6)
message(STATUS "******************************************************************************")
unset(QT_QMAKE_EXECUTABLE)

project(dro)

#had to add this to get cortexm3.s to be processed
enable_language(ASM)

set(STMUSAGES
  "cortexm/stm32/clocks.cpp"
  "cortexm/stm32/bluepill.cpp"
  "cortexm/stm32/gpio.cpp"
  "cortexm/stm32/exti.cpp"
  "cortexm/stm32/flashcontrol.cpp"
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
  "ezcpp/polledtimer.cpp"
  )

set(SOURCES
  "dro-stm.cpp"
  ${CORTEXUSAGES}
  ${STMUSAGES}
  ${EZUSAGES}
  )

#set(LINK_LIBRARIES -lgcc)

add_executable(${PROJECT_NAME} ${SOURCES})
