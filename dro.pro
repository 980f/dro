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

INCLUDEPATH += ../cortexm lpcbase

HEADERS +=\
  nvic.h\
  systick.h\
  cortexm/core_cmInstr.h\
  lpcbase/gpio.h

SOURCES +=\
  systick.cpp\
  nvic.cpp\
  minimath.cpp\
  main.cpp


