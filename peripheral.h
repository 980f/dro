#ifndef PERIPHERAL_H
#define PERIPHERAL_H

/** for a private single instance block */
#define soliton(type, address) type & the ## type = *reinterpret_cast <type *> (address);


#endif // PERIPHERAL_H
