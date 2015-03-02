//to use this you must copy/paste and replace IrqForUart with a usage specific token.
//havta manually resolve the irq number rather than wrestling with the preprocessor.
#if IrqForUart == 1
#define irq4uart 37
#elif IrqForUart == 2
#define irq4uart 38
#elif IrqForUart == 3
#define irq4uart 39
#else
#warning Symbol #IrqForUart not valid or not defined in art.h
#endif
///
