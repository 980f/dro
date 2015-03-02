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

INCLUDEPATH += ../cortexm ../cortexm/stm32 ../ezcpp

HEADERS +=\
  nvic.h\
  systick.h\
  cortexm/core_cmInstr.h \
  cortexm/stm32/gpio.h \
    cortexm/wtf.h \
    cortexm/stm32/p103_board.h \
    ../cortexm/stm32/clocks.h \
    cortexm/stm32/stm32.h \
    ezcpp/eztypes.h

SOURCES +=\
  cortexm/systick.cpp\
  cortexm/nvic.cpp\
  ezcpp/minimath.cpp\
  main.cpp \
  cortexm/stm32/gpio.cpp \
    cortexm/wtf.cpp \
    cortexm/stm32/p103_board.cpp \
    cortexm/stm32/stm32.cpp \
    cortexm/stm32/flashcontrol.cpp \
    ../cortexm/stm32/clocks.cpp

DISTFILES += \
    cortexm/stm32/f103re.ld \
    dro.ld \
    dro.hzp


