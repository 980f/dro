
#include "core-atomic.h"

/** cortex M atomic operations */
#if HOST_SIM
bool atomic_increment(unsigned &alignedDatum){
  ++alignedDatum;
  return false;
}
bool atomic_decrement(unsigned &alignedDatum){
  --alignedDatum;
  return false;
}

#else //real code
//assembler file will supply code
#endif

