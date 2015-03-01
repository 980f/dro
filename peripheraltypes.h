#ifndef PERIPHERALTYPES_H
#define PERIPHERALTYPES_H  1

/**
 *  types used for declaration of peripherals.
 *
 *  The SFR* template classes have a peculiar usage pattern, caused by trying to make all related code be generated inline.
 *  At hardware module definition time one creates a typedef for each field rather than an instance.
 *  At each place of use one creates an instance and then assigns to that instance to write a value or simply reference that instance for a read.
 *  This precludes extern'ing global instances which then must take up real data space (const so not a big cost) and then be accessed via an extra level of indirection compared to what the 'create local instance' can result in.
 *  If we could create a global instance in a header file but get just one actual object created we would do that.
 *
 */
#include <stdint.h> // instead of eztypes, trying to not use too many andyh colloquialisms
#include <stddef.h> // for offsetof(,) used in compile-time computing of register addresses:


typedef volatile uint32_t SFR;
typedef const uint32_t SKIPPED;
// packable byte:
typedef volatile uint8_t SFR8;
typedef volatile uint16_t SFR16;
// todo: 16 bit ones, with word spacers.
// todo: macro for skip block



/** Multiple contiguous bits in a register
 * Note: This creates a class per sf register field, but the compiler should inline all uses making this a compile time burden but a runtime minimalization.
 * Note: 'volatile' isn't used here as it is gratuitous when the variable isn't nominally read multiple times in a function.
 */
template <unsigned sfraddress, int pos, int width = 1> class SFRfield {
  enum {
    mask = ((1 << width) - 1) << pos
  };

  inline SFR &sfr() const {
    return *reinterpret_cast<SFR *>(sfraddress);
  }
private:
  SFRfield(const SFRfield &other) = delete;

public:
  SFRfield(){
  }

  explicit SFRfield(unsigned initlizer){
    this->operator =(initlizer);
  }

  // read
  inline operator uint32_t() const {
    return (sfr() & mask) >> pos;
  }
  // write
  inline void operator =(uint32_t value) const {
    sfr() = ((value << pos) & mask) | (sfr() & ~mask);
  }
};

/** a register that is only 8 bits */
template <unsigned sfraddress> class SFRbyte {
  inline SFR8 &sfr() const {
    return *reinterpret_cast<SFR8 *>(sfraddress);
  }
  public:

  // read
  inline operator uint32_t() const {
    return sfr();
  }
  // write
  inline void operator =(uint32_t value) const {
    sfr() = value;
  }
  //do nothing on declaration of instance
  SFRbyte(){
  }
  //declare and initialize, often the only reference to the object.
  explicit SFRbyte(unsigned initlizer){
    this->operator =(initlizer);
  }

};

/** single bit, ignoring the possibility it is in bitbanded memory (an stm32 special feature it seems).
 *  This is NOT derived from SFRfield as we can do some optimizations that the compiler might miss (or developer might have disabled)*/
template <unsigned sfraddress, int pos> class SFRbit {
  enum {
    mask = 1 << pos
  };

  inline SFR &sfr() const {
    return *reinterpret_cast<SFR *>(sfraddress);
  }
public:
  // read
  inline operator bool() const {
    return (sfr() & mask) != 0;
  }
  // write
  inline void operator =(bool value){
    if(value) {
      sfr() |= mask;
    } else {
      sfr() &= ~mask;
    }
  }
};

/** macro for first template argument for SFRfield and SFRbit, mates to cmsis style type declarations which can't use c bit-fields.*/
#define SFRptr(absaddress, blocktype, member) (absaddress + offsetof(blocktype, member))
//todo: constexpr version
#endif // PERIPHERALTYPES_H
