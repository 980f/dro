TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../ezcpp ../cortexm  ../cortexm/sam3

DEFINES += ARDUINOnot

SOURCES += main.cpp \
    ../ezcpp/bitbanger.cpp \
    ../ezcpp/boolish.cpp \
    ../ezcpp/char.cpp \
    ../ezcpp/cheaptricks.cpp \
    ../ezcpp/cstr.cpp \
    ../ezcpp/minimath.cpp \
    ../ezcpp/polledtimer.cpp \
    ../ezcpp/retriggerablemonostable.cpp \
    ../ezcpp/stopwatch.cpp \
    ../ezcpp/textkey.cpp \
    ../due/streamformatter.cpp \
    ../due/streamprintf.cpp

HEADERS += \
    ../ezcpp/bitbanger.h \
    ../ezcpp/boolish.h \
    ../ezcpp/char.h \
    ../ezcpp/cheaptricks.h \
    ../ezcpp/cstr.h \
    ../ezcpp/eztypes.h \
    ../ezcpp/index.h \
    ../ezcpp/minimath.h \
    ../ezcpp/polledtimer.h \
    ../ezcpp/retriggerablemonostable.h \
    ../ezcpp/stopwatch.h \
    ../ezcpp/tableofpointers.h \
    ../ezcpp/textkey.h \
    ../due/streamformatter.h \
    ../due/streamprintf.h
