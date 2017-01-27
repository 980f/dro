TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

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
#QMAKE_CXXFLAGS += -Wno-error=switch
#QMAKE_CXXFLAGS += -Wno-error=deprecated-declarations
#QMAKE_CXXFLAGS += -Wno-error=strict-aliasing
QMAKE_CXXFLAGS += -Wno-error=unused-parameter
QMAKE_CXXFLAGS += -Wno-error=unused-function

#for debug of macros:
QMAKE_CXXFLAGS += -save-temps

DEFINES += useSTM32=0

INCLUDEPATH += cortexm cortexm/lpc ezcpp

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
    cortexm/core_itm.h \
    cortexm/cruntime.h \
    cortexm/debugio.h \
    cortexm/fifo.h \
    cortexm/nvic.h\
    cortexm/peripheralband.h \
    cortexm/peripheral.h \
    cortexm/peripheraltypes.h \
    cortexm/systick.h\
    cortexm/wtf.h \
    cortexm/lpc/clocks.h \
    cortexm/lpc/gpio.h \
    cortexm/lpc/iocon.h \
    cortexm/lpc/lpc13xx.h \
    cortexm/lpc/lpcperipheral.h \
    cortexm/lpc/p1343_board.h \
    cortexm/lpc/power_api.h \
    cortexm/lpc/startsignal.h \
    cortexm/lpc/syscon.h \
    cortexm/lpc/system_LPC13xx.h \
    cortexm/lpc/uart.h \
    cortexm/lpc/usbinterface.h \
    cortexm/lpc/wdt.h


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
    cortexm/core_itm.cpp \
    cortexm/coredebug.cpp \
    cortexm/systick.cpp \
    cortexm/nvic.cpp \
    cortexm/wtf.cpp \
    cortexm/clockstarter.cpp \
    cortexm/cstartup.cpp \
    cortexm/lpc/wdt.cpp \
    cortexm/lpc/clocks.cpp \
    cortexm/lpc/gpio.cpp \
    cortexm/lpc/uart.cpp \
    cortexm/lpc/iocon.cpp \
    cortexm/lpc/lpc13xx.cpp \
    cortexm/lpc/startsignal.cpp \
    cortexm/lpc/syscon.cpp \
    cortexm/lpc/system_LPC13xx.cpp \
    cortexm/lpc/usbinterface.cpp \
    cortexm/lpc/p1343_board.cpp \
    p1343-dro.cpp

DISTFILES += \
    dro-lpc.ld \
    cortexm/lpc/lpc1343.ld \
    cortexm/cortexm3.s \
    cortexm/cortexm.ld


#end of file.
