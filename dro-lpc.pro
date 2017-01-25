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
    cortexm/nvic.h\
    cortexm/systick.h\
    cortexm/core_cmInstr.h \
    cortexm/wtf.h \
    ezcpp/eztypes.h \
    cortexm/peripheral.h \
    ezcpp/polledtimer.h \
    cortexm/cruntime.h \
    ezcpp/boolish.h \
    ezcpp/minimath.h \
    ezcpp/cheaptricks.h \
    cortexm/lpc/uart.h \
    cortexm/lpc/gpio.h \
    cortexm/lpc/startsignal.h \
    packdatatest.h \
    cortexm/lpc/usbinterface.h \
    ezcpp/tableofpointers.h \
    arduino/due/digitalpin.h \
    arduino/due/pinclass.h \
    cortexm/lpc/clocks.h \
    cortexm/lpc/iocon.h \
    cortexm/lpc/lpc13xx.h \
    cortexm/lpc/lpcperipheral.h \
    cortexm/lpc/p1343_board.h \
    cortexm/lpc/power_api.h \
    cortexm/lpc/syscon.h \
    cortexm/lpc/system_LPC13xx.h \
    cortexm/lpc/wdt.h \
    cortexm/stm32/adc.h \
    cortexm/stm32/afio.h \
    cortexm/stm32/debugger.h \
    cortexm/stm32/dma.h \
    cortexm/stm32/dmabuffereduart.h \
    cortexm/stm32/exti.h \
    cortexm/stm32/flashcontrol.h \
    cortexm/stm32/gpio.h \
    cortexm/stm32/i2c.h \
    cortexm/stm32/i2cswmaster.h \
    cortexm/stm32/irq4uart.h \
    cortexm/stm32/p103_board.h \
    cortexm/stm32/pairedoneshots.h \
    cortexm/stm32/peripheral.h \
    cortexm/stm32/pvd.h \
    cortexm/stm32/pwmoutput.h \
    cortexm/stm32/rtc.h \
    cortexm/stm32/shadow.h \
    cortexm/stm32/spi.h \
    cortexm/stm32/stm32.h \
    cortexm/stm32/timer.h \
    cortexm/stm32/uart.h \
    cortexm/clocks.h \
    cortexm/core-atomic.h \
    cortexm/core_cm3.h \
    cortexm/core_cmFunc.h \
    cortexm/core_itm.h \
    cortexm/coredebug.h \
    cortexm/debugio.h \
    cortexm/fifo.h \
    cortexm/peripheraltypes.h \
    ezcpp/bitbanger.h \
    ezcpp/bundler.h \
    ezcpp/circularpointer.h \
    ezcpp/limitedcounter.h \
    ezcpp/limitedpointer.h \
    cortexm/peripheralband.h


SOURCES +=\
    cortexm/systick.cpp \
    cortexm/nvic.cpp \
    cortexm/cstartup.cpp \
    cortexm/wtf.cpp \
    ezcpp/minimath.cpp \
    ezcpp/polledtimer.cpp \
    ezcpp/boolish.cpp \
    cortexm/lpc/p1343_board.cpp \
    cortexm/lpc/clocks.cpp \
    cortexm/lpc/gpio.cpp \
    cortexm/clockstarter.cpp \
    ezcpp/cheaptricks.cpp \
    cortexm/lpc/uart.cpp \
    cortexm/fifo.cpp \
    cortexm/core-atomic.cpp \
    p1343-dro.cpp \
    cortexm/lpc/iocon.cpp \
    cortexm/lpc/lpc13xx.cpp \
    cortexm/lpc/main.cpp \
    cortexm/lpc/startsignal.cpp \
    cortexm/lpc/syscon.cpp \
    cortexm/lpc/system_LPC13xx.cpp \
    cortexm/lpc/usbinterface.cpp \
    cortexm/lpc/wdt.cpp \
    cortexm/stm32/adc.cpp \
    cortexm/stm32/afio.cpp \
    cortexm/stm32/clocks.cpp \
    cortexm/stm32/dma.cpp \
    cortexm/stm32/dmabuffereduart.cpp \
    cortexm/stm32/exti.cpp \
    cortexm/stm32/flashcontrol.cpp \
    cortexm/stm32/gpio.cpp \
    cortexm/stm32/i2c.cpp \
    cortexm/stm32/p103_board.cpp \
    cortexm/stm32/pairedoneshots.cpp \
    cortexm/stm32/pvd.cpp \
    cortexm/stm32/pwmoutput.cpp \
    cortexm/stm32/rtc.cpp \
    cortexm/stm32/spi.cpp \
    cortexm/stm32/stm32.cpp \
    cortexm/stm32/timer.cpp \
    cortexm/stm32/uart.cpp \
    cortexm/core_cm3.cpp \
    cortexm/core_itm.cpp \
    cortexm/coredebug.cpp \
    cortexm/nvicstub.cpp \
    ezcpp/bitbanger.cpp \
    ezcpp/circularpointer.cpp \
    ezcpp/limitedcounter.cpp \
    ezcpp/limitedpointer.cpp \
    main.cpp


DISTFILES += \
    dro-lpc.ld \
    cortexm/lpc/lpc1343.ld \
    dro.hzp \
    cortexm/lpc/peripherals.ld \
    cortexm/cortexm3.s \
    cortexm/cortexm.ld \
    cortexm/sam3/sam3x8e.ld


#end of file.
