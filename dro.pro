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
QMAKE_CXXFLAGS += -Wno-error=attributes
#QMAKE_CXXFLAGS += -Wno-error=switch
QMAKE_CXXFLAGS += -Wno-error=deprecated-declarations
QMAKE_CXXFLAGS += -Wno-error=strict-aliasing
QMAKE_CXXFLAGS += -Wno-error=unused-parameter
QMAKE_CXXFLAGS += -Wno-error=unused-function

#using O2 with batch build
#QMAKE_CXXFLAGS += -O0
#for debug of macros:
#QMAKE_CXXFLAGS += -save-temps

INCLUDEPATH += lpcbase

HEADERS +=\
  nvic.h\
  systick.h\
  lpcbase/core_cmInstr.h\
  lpcbase/gpio.h

SOURCES +=\
  systick.cpp\
  nvic.cpp\
  minimath.cpp\
  main.cpp \
    lpcbase/clocks.cpp \
    lpcbase/stubs.c


