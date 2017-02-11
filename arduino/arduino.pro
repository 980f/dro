TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fpermissive

DEFINES += ARDUINO

INCLUDEPATH += \
  ../arduino/cortexm/sam3 \
  ../arduino/cortexm/ \
  ../ezcpp/

SOURCES += \
    ../arduino/cortexm/sam3/arduinodue.cpp \
    ../arduino/cortexm/sam3/clockgenerator.cpp \
    ../arduino/cortexm/sam3/feature.cpp \
    ../arduino/cortexm/sam3/gpio.cpp \
    ../arduino/cortexm/sam3/uart.cpp \
    ../arduino/cortexm/core-atomic.cpp \
    ../arduino/cortexm/core_cm3.cpp \
    ../arduino/cortexm/fifo.cpp \
    ../arduino/cortexm/wtf.cpp \
    ../ezcpp/bitbanger.cpp \
    ../ezcpp/boolish.cpp \
    ../ezcpp/cheaptricks.cpp \
    ../ezcpp/circularpointer.cpp \
    ../ezcpp/limitedcounter.cpp \
    ../ezcpp/limitedpointer.cpp \
    ../ezcpp/minimath.cpp \
    ../ezcpp/polledtimer.cpp \
    ../ezcpp/retriggerablemonostable.cpp \
    ../ezcpp/stopwatch.cpp \
    due/sam3xcounter.cpp

HEADERS += \
    due/due.ino \
    due/interruptGuard.h \
    due/interruptPin.h \
    due/pinclass.h \
    due/sam3xcounter.h \
    digitalpin.h \
    ../arduino/cortexm/sam3/gpio.h \
    ../arduino/cortexm/sam3/samperipheral.h \
    ../arduino/cortexm/sam3/uart.h \
    ../arduino/cortexm/core-atomic.h \
    ../arduino/cortexm/core_cm3.h \
    ../arduino/cortexm/core_cmFunc.h \
    ../arduino/cortexm/core_cmInstr.h \
    ../arduino/cortexm/core_itm.h \
    ../arduino/cortexm/cruntime.h \
    ../arduino/cortexm/debugio.h \
    ../arduino/cortexm/fifo.h \
    ../arduino/cortexm/peripheralband.h \
    ../arduino/cortexm/peripheraltypes.h \
    ../arduino/cortexm/wtf.h \
    ../ezcpp/bitbanger.h \
    ../ezcpp/boolish.h \
    ../ezcpp/bundler.h \
    ../ezcpp/cheaptricks.h \
    ../ezcpp/circularpointer.h \
    ../ezcpp/eztypes.h \
    ../ezcpp/limitedcounter.h \
    ../ezcpp/limitedpointer.h \
    ../ezcpp/minimath.h \
    ../ezcpp/polledtimer.h \
    ../ezcpp/retriggerablemonostable.h \
    ../ezcpp/stopwatch.h \
    ../ezcpp/tableofpointers.h \
    ../interruptGuard.h \
    ../interruptPin.h \
    ../pinclass.h \
    ../sam3xcounter.h \
    ../arduino/cortexm/nvic.h


