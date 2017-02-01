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

QMAKE_CXXFLAGS += -Werror

#we use attributes fairly heavily in firmware, could pragma just those files but none of us use attributes gratuitously or casually.
QMAKE_CXXFLAGS += -Wno-error=attributes
QMAKE_CXXFLAGS += -Wno-error=unused-parameter
QMAKE_CXXFLAGS += -Wno-error=unused-function

#for debug of macros:
QMAKE_CXXFLAGS += -save-temps



INCLUDEPATH += cortexm cortexm/sam3 ezcpp

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
    cortexm/sam3/gpio.h \
    cortexm/sam3/samperipheral.h \
    cortexm/sam3/feature.h \
    cortexm/sam3/arduinodue.h \
    cortexm/sam3/clockgenerator.h \
    cortexm/sam3/uart.h

SOURCES +=\
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
    dro-due.cpp \
    cortexm/sam3/gpio.cpp \
    cortexm/sam3/feature.cpp \
    cortexm/core_cmfunc.cpp \
    cortexm/sam3/arduinodue.cpp \
    cortexm/sam3/clockgenerator.cpp \
    cortexm/sam3/uart.cpp

DISTFILES += \
    dro-due.ld \
    cortexm/sam3/sam3x8e.ld \
    cortexm/cortexm3.s \
    cortexm/cortexm.ld


#end of file.
