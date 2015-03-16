TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -std=c99
QMAKE_CFLAGS +=   -Wall
QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS += -funsigned-char
QMAKE_CXXFLAGS += -fdiagnostics-show-option

QMAKE_CXXFLAGS += -Werror
#we use attributes fairly heavily in firmware, could pragme just those files but none of us use attributes gratuitously or casually.
QMAKE_CXXFLAGS += -Wno-error=attributes
#QMAKE_CXXFLAGS += -Wno-error=switch
#QMAKE_CXXFLAGS += -Wno-error=deprecated-declarations
#QMAKE_CXXFLAGS += -Wno-error=strict-aliasing
QMAKE_CXXFLAGS += -Wno-error=unused-parameter
QMAKE_CXXFLAGS += -Wno-error=unused-function

#for debug of macros:
#QMAKE_CXXFLAGS += -save-temps

DEFINES += useSTM32=0

INCLUDEPATH += cortexm cortexm/lpc ezcpp

HEADERS +=\
    cortexm/nvic.h\
    cortexm/systick.h\
    cortexm/core_cmInstr.h \
    cortexm/wtf.h \
    ezcpp/eztypes.h \
    cortexm/peripheral.h \
    ezcpp/bundler.h \
    ezcpp/polledtimer.h \
    cortexm/cruntime.h \
    ezcpp/boolish.h \
    ezcpp/minimath.h \
    ezcpp/cheapTricks.h \
    cortexm/lpc/uart.h


SOURCES +=\
    main.cpp \
    cortexm/systick.cpp \
    cortexm/nvic.cpp \
    cortexm/cstartup.cpp \
    cortexm/wtf.cpp \
    ezcpp/minimath.cpp \
    ezcpp/bundler.cpp \
    ezcpp/polledtimer.cpp \
    ezcpp/boolish.cpp \
    cortexm/lpc/p1343_board.cpp \
    cortexm/lpc/clocks.cpp \
    cortexm/lpc/gpio.cpp \
    cortexm/clockstarter.cpp \
    ezcpp/cheapTricks.cpp \
    cortexm/lpc/uart.cpp \
    cortexm/fifo.cpp \
    cortexm/core-atomic.cpp


DISTFILES += \
    dro-lpc.ld \
    cortexm/lpc/lpc1343.ld \
    dro.hzp \
    cortexm/lpc/peripherals.ld


#end of file.
