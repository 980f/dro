TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fpermissive

DEFINES += ARDUINO

INCLUDEPATH += \
  ../../Arduino/libraries/ezcortexm/src/cortexm/sam3 \
  ../../Arduino/libraries/ezcortexm/src/cortexm/ \
  ../../Arduino/libraries/ezcortexm/src/ezcpp/

SOURCES += \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/arduinodue.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/clockgenerator.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/feature.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/gpio.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/uart.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core-atomic.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core_cm3.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/fifo.cpp \
    ../../Arduino/libraries/ezcortexm/src/cortexm/wtf.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/bitbanger.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/boolish.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/cheaptricks.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/circularpointer.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/limitedcounter.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/limitedpointer.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/minimath.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/polledtimer.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/retriggerablemonostable.cpp \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/stopwatch.cpp \
    due/sam3xcounter.cpp

HEADERS += \
    due/due.ino \
    due/interruptGuard.h \
    due/interruptPin.h \
    due/pinclass.h \
    due/sam3xcounter.h \
    digitalpin.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/arduinodue.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/clockgenerator.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/feature.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/gpio.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/samperipheral.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/sam3/uart.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core-atomic.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core_cm3.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core_cmFunc.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core_cmInstr.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/core_itm.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/cruntime.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/debugio.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/fifo.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/peripheralband.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/peripheraltypes.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/wtf.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/bitbanger.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/boolish.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/bundler.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/cheaptricks.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/circularpointer.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/eztypes.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/limitedcounter.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/limitedpointer.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/minimath.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/polledtimer.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/retriggerablemonostable.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/stopwatch.h \
    ../../Arduino/libraries/ezcortexm/src/ezcpp/tableofpointers.h \
    ../../Arduino/libraries/ezcortexm/src/interruptGuard.h \
    ../../Arduino/libraries/ezcortexm/src/interruptPin.h \
    ../../Arduino/libraries/ezcortexm/src/pinclass.h \
    ../../Arduino/libraries/ezcortexm/src/sam3xcounter.h \
    ../../Arduino/libraries/ezcortexm/src/cortexm/nvic.h


