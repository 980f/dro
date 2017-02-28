TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../ezcpp ../cortexm  ../cortexm/sam3
INCLUDEPATH += /d/bin/arduino/hardware/arduino/sam/cores/arduino/

DEFINES += ARDUINOnot

SOURCES += \
    ../ezcpp/bitbanger.cpp \
    ../ezcpp/boolish.cpp \
    ../ezcpp/char.cpp \
    ../ezcpp/cheaptricks.cpp \
    ../ezcpp/cstr.cpp \
    ../ezcpp/minimath.cpp \
    ../ezcpp/polledtimer.cpp \
    ../ezcpp/retriggerablemonostable.cpp \
    ../ezcpp/stopwatch.cpp \
    ../ezcpp/textkey.cpp \
    ../due/streamformatter.cpp \
    ../due/streamprintf.cpp \
    ../ezcpp/softpwm.cpp \
    ../cortexm/lpc/baudsearch.cpp \
    ../cortexm/lpc/clocks.cpp \
    ../cortexm/lpc/gpio.cpp \
    ../cortexm/lpc/iocon.cpp \
    ../cortexm/lpc/lpc13xx.cpp \
    ../cortexm/lpc/p1343_board.cpp \
    ../cortexm/lpc/syscon.cpp \
    ../cortexm/lpc/uart.cpp \
    ../../dro-lpc.cpp

HEADERS += \
    ../ezcpp/bitbanger.h \
    ../ezcpp/boolish.h \
    ../ezcpp/char.h \
    ../ezcpp/cheaptricks.h \
    ../ezcpp/cstr.h \
    ../ezcpp/eztypes.h \
    ../ezcpp/index.h \
    ../ezcpp/minimath.h \
    ../ezcpp/polledtimer.h \
    ../ezcpp/retriggerablemonostable.h \
    ../ezcpp/stopwatch.h \
    ../ezcpp/tableofpointers.h \
    ../ezcpp/textkey.h \
    ../due/streamformatter.h \
    ../due/streamprintf.h \
    ../due/interruptPin.h \
    ../due/due.ino \
    ../ezcpp/softpwm.h \
    ../cortexm/lpc/baudsearch.h \
    ../cortexm/lpc/clocks.h \
    ../cortexm/lpc/gpio.h \
    ../cortexm/lpc/iocon.h \
    ../cortexm/lpc/lpc13xx.h \
    ../cortexm/lpc/lpcperipheral.h \
    ../cortexm/lpc/p1343_board.h \
    ../cortexm/lpc/pinirq.h \
    ../cortexm/lpc/syscon.h \
    ../cortexm/lpc/uart.h
