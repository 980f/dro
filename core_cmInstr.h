#pragma once

/* GNU gcc specific syntax for some special instructions */

//some people do this:
#define INTRINSICALLY [[always_inline]] static inline
//others do this, not sure which is better or if they even differ ...
#define URGENTLY __attribute__((always_inline))

#if __linux__
//then just compiling for syntax checking.
#define MNE(mne)
#else
//#define SINGLEOP(mne)  INTRINSICALLY unsigned __ ## mne(unsigned value){uint32_t result; __asm volatile (# mne " %0, %1" : "=r" (result) : "r" (value));  return result; }
// the above despite all the inline keywords was made into a callable function,so:
#define MNE(mne) __asm volatile (# mne)

#endif
