TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CFLAGS += -std=c99
QMAKE_CFLAGS +=   -Wall
QMAKE_CXXFLAGS += -std=c++14

QMAKE_CXXFLAGS += -funsigned-char
QMAKE_CXXFLAGS += -fdiagnostics-show-option

#finally had to give in and state this so that we can do address calculations:
QMAKE_CXXFLAGS += -fpermissive
#and that wasn't fucking good enough so add a pragma where needed (wasn't good enough by itself)

#QMAKE_CXXFLAGS += -Werror

#we use attributes fairly heavily in firmware, could pragma just those files but none of us use attributes gratuitously or casually.
QMAKE_CXXFLAGS += -Wno-error=attributes
QMAKE_CXXFLAGS += -Wno-error=unused-parameter
QMAKE_CXXFLAGS += -Wno-error=unused-function

#for debug of macros:
QMAKE_CXXFLAGS += -save-temps



INCLUDEPATH += cortexm cortexm/stm32 ezcpp

HEADERS +=\
    ezcpp/bitbanger.h \
    ezcpp/boolish.h \
    ezcpp/bundler.h \
    ezcpp/cheaptricks.h \
    ezcpp/circularpointer.h \
    ezcpp/eztypes.h \
    ezcpp/limitedcounter.h \
    ezcpp/limitedpointer.h \
    ezcpp/minimath.h \
    ezcpp/polledtimer.h \
    ezcpp/tableofpointers.h \
    cortexm/clocks.h \
    cortexm/core-atomic.h \
    cortexm/core_cm3.h \
    cortexm/core_cmFunc.h \
    cortexm/core_cmInstr.h \
    cortexm/coredebug.h \
    cortexm/cruntime.h \
    cortexm/debugio.h \
    cortexm/fifo.h \
    cortexm/nvic.h\
    cortexm/peripheralband.h \
    cortexm/peripheral.h \
    cortexm/peripheraltypes.h \
    cortexm/systick.h\
    cortexm/wtf.h \
    cortexm/stm32/afio.h \
    cortexm/stm32/exti.h \
    cortexm/stm32/flashcontrol.h \
    cortexm/stm32/gpio.h \
    cortexm/stm32/p103_board.h \
    cortexm/stm32/peripheral.h \
    cortexm/stm32/stm32.h \
    cortexm/stm32/timer.h \
    cortexm/stm32/uart.h


SOURCES +=\
  p103.cpp \
  ezcpp/cheaptricks.cpp \
    ezcpp/minimath.cpp \
    ezcpp/polledtimer.cpp \
    ezcpp/boolish.cpp \
    ezcpp/bitbanger.cpp \
    ezcpp/circularpointer.cpp \
    ezcpp/limitedcounter.cpp \
    ezcpp/limitedpointer.cpp \
    cortexm/core-atomic.cpp \
    cortexm/fifo.cpp \
    cortexm/core_cm3.cpp \
    cortexm/coredebug.cpp \
    cortexm/systick.cpp \
    cortexm/nvic.cpp \
    cortexm/wtf.cpp \
    cortexm/clockstarter.cpp \
    cortexm/cstartup.cpp \
    cortexm/core_cmfunc.cpp \
    cortexm/stm32/afio.cpp \
    cortexm/stm32/clocks.cpp \
    cortexm/stm32/flashcontrol.cpp \
    cortexm/stm32/gpio.cpp \
    cortexm/stm32/p103_board.cpp \
    cortexm/stm32/stm32.cpp \
    cortexm/stm32/timer.cpp \
    cortexm/stm32/uart.cpp

DISTFILES += \
    p103.ld \
    cortexm/cortexm3.s \
    cortexm/cortexm.ld \
    cortexm/mkIrqs \
    cortexm/stm32/f103rb.ld


#end of file.
