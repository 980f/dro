#repo: https://github.com/greiman/SdFat.git =>sdfat

DEFINES += __STM32F1__=103
DEFINES += ARDUINO

#which hardware port, if any. Leave undef for softspi.
DEFINES +=SDCARD_SPI=SPI1

INCLUDEPATH += sdfat/src

SOURCES += \
    sdfat/src/FatLib/FatFile.cpp \
    sdfat/src/FatLib/FatFileLFN.cpp \
    sdfat/src/FatLib/FatFilePrint.cpp \
    sdfat/src/FatLib/FatFileSFN.cpp \
    sdfat/src/FatLib/FatVolume.cpp \
    sdfat/src/FatLib/FmtNumber.cpp \
    sdfat/src/FatLib/fstream.cpp \
    sdfat/src/FatLib/istream.cpp \
    sdfat/src/FatLib/ostream.cpp \
    sdfat/src/FatLib/StdioStream.cpp \
    sdfat/src/SdCard/SdioCardEX.cpp \
    sdfat/src/SdCard/SdioTeensy.cpp \
    sdfat/src/SdCard/SdSpiCard.cpp \
    sdfat/src/SdCard/SdSpiCardEX.cpp \
    sdfat/src/SpiDriver/SdSpiESP8266.cpp \
    sdfat/src/SpiDriver/SdSpiSAM3X.cpp \
    sdfat/src/SpiDriver/SdSpiSTM32.cpp \
    sdfat/src/SpiDriver/SdSpiTeensy3.cpp \
    sdfat/src/MinimumSerial.cpp

HEADERS += \
    sdfat/src/FatLib/ArduinoFiles.h \
    sdfat/src/FatLib/ArduinoStream.h \
    sdfat/src/FatLib/BaseBlockDriver.h \
    sdfat/src/FatLib/bufstream.h \
    sdfat/src/FatLib/FatApiConstants.h \
    sdfat/src/FatLib/FatFile.h \
    sdfat/src/FatLib/FatFileSystem.h \
    sdfat/src/FatLib/FatLib.h \
    sdfat/src/FatLib/FatLibConfig.h \
    sdfat/src/FatLib/FatStructs.h \
    sdfat/src/FatLib/FatVolume.h \
    sdfat/src/FatLib/FmtNumber.h \
    sdfat/src/FatLib/fstream.h \
    sdfat/src/FatLib/ios.h \
    sdfat/src/FatLib/iostream.h \
    sdfat/src/FatLib/istream.h \
    sdfat/src/FatLib/ostream.h \
    sdfat/src/FatLib/StdioStream.h \
    sdfat/src/SdCard/SdInfo.h \
    sdfat/src/SdCard/SdioCard.h \
    sdfat/src/SdCard/SdSpiCard.h \
    sdfat/src/SpiDriver/boards/AvrDevelopersGpioPinMap.h \
    sdfat/src/SpiDriver/boards/BobuinoGpioPinMap.h \
    sdfat/src/SpiDriver/boards/GpioPinMap.h \
    sdfat/src/SpiDriver/boards/LeonardoGpioPinMap.h \
    sdfat/src/SpiDriver/boards/MegaGpioPinMap.h \
    sdfat/src/SpiDriver/boards/SleepingBeautyGpioPinMap.h \
    sdfat/src/SpiDriver/boards/Standard1284GpioPinMap.h \
    sdfat/src/SpiDriver/boards/Teensy2GpioPinMap.h \
    sdfat/src/SpiDriver/boards/Teensy2ppGpioPinMap.h \
    sdfat/src/SpiDriver/boards/UnoGpioPinMap.h \
    sdfat/src/SpiDriver/DigitalPin.h \
    sdfat/src/SpiDriver/SdSpiBaseDriver.h \
    sdfat/src/SpiDriver/SdSpiDriver.h \
    sdfat/src/SpiDriver/SoftSPI.h \
    sdfat/src/BlockDriver.h \
    sdfat/src/FreeStack.h \
    sdfat/src/MinimumSerial.h \
    sdfat/src/SdFat.h \
    sdfat/src/SdFatConfig.h \
    sdfat/src/SysCall.h
