cmake_minimum_required(VERSION 3.29)

add_executable(dro
    dro-stm.cpp

    cortexm/cortexm3.s
    cortexm/clockstarter.cpp
    cortexm/nvic.cpp
    cortexm/systick.cpp

    cortexm/fifo.cpp
    cortexm/wtf.cpp

    cortexm/stm32/stm32.cpp
    cortexm/stm32/gpio.cpp
    cortexm/stm32/afio.cpp
    cortexm/stm32/exti.cpp
    cortexm/stm32/clocksf1.cpp
    cortexm/stm32/dmaf1.cpp
    cortexm/stm32/flashf1.cpp
    cortexm/stm32/timer.cpp
    cortexm/stm32/uart.cpp

    ezcpp/bitbasher.cpp
    ezcpp/sharedtimer.cpp
    ezcpp/quadraturecounter.cpp
    ezcpp/stopwatch.cpp
    cortexm/stm32/bluepill.cpp
    cortexm/stm32/bluepill.h
)

add_compile_options(-fpermissive)

target_compile_options(dro PUBLIC -fpermissive)

include_directories(
    .
    cortexm
    cortexm/stm32
    ezcpp
)
