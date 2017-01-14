cmake_minimum_required(VERSION 3.3)
project(dro)

set(CMAKE_CXX_FLAGS "-fpermissive ${CMAKE_CXX_FLAGS} -std=c++11 ")

include_directories(cortexm ezcpp)

set(SOURCE_FILES
    cortexm/lpc/clocks.cpp
    cortexm/lpc/clocks.h
    cortexm/lpc/gpio.cpp
    cortexm/lpc/gpio.h
    cortexm/lpc/iocon.cpp
    cortexm/lpc/iocon.h
    cortexm/lpc/lpc13xx.cpp
    cortexm/lpc/lpc13xx.h
    cortexm/lpc/lpcperipheral.h
    cortexm/lpc/main.cpp
    cortexm/lpc/p1343_board.cpp
    cortexm/lpc/p1343_board.h
    cortexm/lpc/power_api.h
    cortexm/lpc/startsignal.cpp
    cortexm/lpc/startsignal.h
    cortexm/lpc/syscon.cpp
    cortexm/lpc/syscon.h
    cortexm/lpc/system_LPC13xx.cpp
    cortexm/lpc/system_LPC13xx.h
    cortexm/lpc/uart.cpp
    cortexm/lpc/uart.h
    cortexm/lpc/wdt.cpp
    cortexm/lpc/wdt.h
    cortexm/clocks.h
    cortexm/clockstarter.cpp
    cortexm/core-atomic.cpp
    cortexm/core-atomic.h
    cortexm/core_cm3.cpp
    cortexm/core_cm3.h
    cortexm/core_cmFunc.h
    cortexm/core_cmInstr.h
    cortexm/core_itm.cpp
    cortexm/core_itm.h
    cortexm/coredebug.cpp
    cortexm/coredebug.h
    cortexm/cruntime.h
    cortexm/cstartup.cpp
    cortexm/debugio.h
    cortexm/fifo.cpp
    cortexm/fifo.h
    cortexm/nvic.cpp
    cortexm/nvic.h
    cortexm/peripheraltypes.h
    cortexm/systick.cpp
    cortexm/systick.h
    cortexm/wtf.cpp
    cortexm/wtf.h
    ezcpp/bitbanger.cpp
    ezcpp/bitbanger.h
    ezcpp/boolish.cpp
    ezcpp/boolish.h
    ezcpp/bundler.cpp
    ezcpp/bundler.h
    ezcpp/cheapTricks.cpp
    ezcpp/cheapTricks.h
    ezcpp/circularpointer.cpp
    ezcpp/circularpointer.h
    ezcpp/eztypes.h
    ezcpp/limitedcounter.cpp
    ezcpp/limitedcounter.h
    ezcpp/limitedpointer.cpp
    ezcpp/limitedpointer.h
    ezcpp/minimath.cpp
    ezcpp/minimath.h
    ezcpp/polledtimer.cpp
    ezcpp/polledtimer.h
    main.cpp
    p1343-dro.cpp)

add_executable(dro ${SOURCE_FILES})