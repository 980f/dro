import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        consoleApplication: true
        files: [
            "../cortexm/clocks.h",
            "../cortexm/clockstarter.cpp",
            "../cortexm/core-atomic.cpp",
            "../cortexm/core-atomic.h",
            "../cortexm/core_cm3.cpp",
            "../cortexm/core_cm3.h",
            "../cortexm/core_cmInstr.h",
            "../cortexm/cortexm.ld",
            "../cortexm/cortexm3.s",
            "../cortexm/cruntime.h",
            "../cortexm/cstartup.cpp",
            "../cortexm/nvic.cpp",
            "../cortexm/nvic.h",
            "../cortexm/peripheralband.h",
            "../cortexm/peripheraltypes.h",
            "../cortexm/stm32/afio.cpp",
            "../cortexm/stm32/afio.h",
            "../cortexm/stm32/clocks.cpp",
            "../cortexm/stm32/dma.cpp",
            "../cortexm/stm32/dma.h",
            "../cortexm/stm32/flashcontrol.cpp",
            "../cortexm/stm32/flashcontrol.h",
            "../cortexm/stm32/gpio.cpp",
            "../cortexm/stm32/gpio.h",
            "../cortexm/stm32/l452re.ld",
            "../cortexm/stm32/peripheral.h",
            "../cortexm/stm32/spi.cpp",
            "../cortexm/stm32/spi.h",
            "../cortexm/stm32/stm32.cpp",
            "../cortexm/stm32/stm32.h",
            "../cortexm/stm32/timer.cpp",
            "../cortexm/stm32/timer.h",
            "../cortexm/stm32/uartl4.cpp",
            "../cortexm/stm32/uartl4.h",
            "../cortexm/systick.cpp",
            "../cortexm/systick.h",
            "../cortexm/wtf.cpp",
            "../cortexm/wtf.h",
            "main.cpp",
            "../dro-l452.ld",
        ]

        cpp.cxxFlags: [ "-nostdlib", "-nostartfiles" , "-fno-exceptions" ]
        cpp.includePaths: ["/d/bin/gccarm721/lib/gcc/arm-none-eabi/7.2.1/include","..", "../cortexm","../ezcpp"]
        cpp.linkerFlags: ["-nostdlib" , "-nostartfiles" ,  "--script=/d/work/dro/cortexm/stm32/l452re.ld" , "--script=/d/work/dro/cortexm/cortexm.ld" ,"-M"]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
