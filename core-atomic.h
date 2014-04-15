#ifndef COREATOMIC_H
#define COREATOMIC_H

/** @return whether the alignedDatum was incremented.
 * THe most common reason for a failure would be that an interrupt occurred. */
bool atomic_increment(unsigned &alignedDatum);

/** @return whether the alignedDatum was decremented */
bool atomic_decrement(unsigned &alignedDatum);


#endif // COREATOMIC_H
