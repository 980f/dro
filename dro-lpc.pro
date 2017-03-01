TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../ezcpp ../cortexm  ../cortexm/lpc


SOURCES += \
    ezcpp/bitbanger.cpp \
    ezcpp/boolish.cpp \
    ezcpp/char.cpp \
    ezcpp/cheaptricks.cpp \
    ezcpp/cstr.cpp \
    ezcpp/minimath.cpp \
    ezcpp/polledtimer.cpp \
    ezcpp/retriggerablemonostable.cpp \
    ezcpp/stopwatch.cpp \
    ezcpp/textkey.cpp \
    cortexm/lpc/baudsearch.cpp \
    cortexm/lpc/clocks.cpp \
    cortexm/lpc/gpio.cpp \
    cortexm/lpc/iocon.cpp \
    cortexm/lpc/lpc13xx.cpp \
    cortexm/lpc/p1343_board.cpp \
    cortexm/lpc/syscon.cpp \
    cortexm/lpc/uart.cpp \
    dro-lpc.cpp \
    cortexm/clockstarter.cpp \
    cortexm/core-atomic.cpp \
    cortexm/fifo.cpp \
    cortexm/nvic.cpp \
    cortexm/systick.cpp \
    cortexm/wtf.cpp

HEADERS += \
    ezcpp/bitbanger.h \
    ezcpp/boolish.h \
    ezcpp/char.h \
    ezcpp/cheaptricks.h \
    ezcpp/cstr.h \
    ezcpp/eztypes.h \
    ezcpp/index.h \
    ezcpp/minimath.h \
    ezcpp/polledtimer.h \
    ezcpp/retriggerablemonostable.h \
    ezcpp/stopwatch.h \
    ezcpp/tableofpointers.h \
    ezcpp/textkey.h \
    cortexm/lpc/baudsearch.h \
    cortexm/lpc/clocks.h \
    cortexm/lpc/gpio.h \
    cortexm/lpc/iocon.h \
    cortexm/lpc/lpc13xx.h \
    cortexm/lpc/lpcperipheral.h \
    cortexm/lpc/p1343_board.h \
    cortexm/lpc/pinirq.h \
    cortexm/lpc/syscon.h \
    cortexm/lpc/uart.h \
    cortexm/core_cmInstr.h \
    cortexm/core-atomic.h \
    cortexm/fifo.h \
    cortexm/nvic.h \
    cortexm/peripheralband.h \
    cortexm/peripheraltypes.h \
    cortexm/systick.h \
    cortexm/wtf.h
