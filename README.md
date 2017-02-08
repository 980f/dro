# cortexm
C++ application base for cortexM* microcontrollers.
The goal is to have hardware bits look like booleans.
The biggest improvement over what most IDE's generate is a linker script that allows for ordering of static initializations across all modules, for data not just init code.
That script also allows you to assemble a table of items each declared privately in their own module, but accessible as an array.

This is all investigative work, use entirely at your own risk.
