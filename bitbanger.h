#ifndef BITBANGER_H
#define BITBANGER_H

/** bit and bitfield setting and getting.*/

/** trying to bind the address as a template arg runs afould of the address not being knowable at compile time*/
template <unsigned lsb, unsigned msb> class BitField {
  enum {
    mask = ((1 << (msb + 1)) - (1 << lsb)) // aligned mask
  };
  unsigned &item;
public:
  BitField(unsigned &item): item(item){
  }
  operator unsigned() const {
    return (item & mask) >> lsb;
  }
  void operator =(unsigned value) const {
    item = (item & ~mask) | ((value << lsb) & mask);
  }
};


/** discontiguous bits, used for bitmmasking */
template <unsigned ... list> struct BitWad;

template <unsigned pos> struct BitWad<pos> {
  enum { mask = 1<<pos };
public:

  static bool exactly(unsigned varble,unsigned match){
    return (mask & varble) == (mask&match);//added mask to second term to allow for lazy programming
  }

  static bool all(unsigned varble){
    return (mask & varble) == mask;
  }

  static bool any(unsigned varble){
    return mask & varble;
  }

  static bool none(unsigned varble){
    return (mask & varble) == 0;
  }

};

template <unsigned pos, unsigned ... poss> struct BitWad<pos, poss ...> {
  enum { mask = (1 << pos) + BitWad<poss...>::mask };

public:
  static bool exactly(unsigned varble,unsigned match){
    return (mask & varble) == (mask&match);//added mask to second term to allow for lazy programming
  }

  static bool all(unsigned varble){
    return (mask & varble) == mask;
  }

  static bool any(unsigned varble){
    return mask & varble;
  }

  static bool none(unsigned varble){
    return (mask & varble) == 0;
  }

};

///trying to compact BitWad:
//template <unsigned mask> struct BitMasker {

//public:
//  static bool exactly(unsigned varble,unsigned match){
//    return (mask & varble) == (mask&match);//added mask to second term to allow for lazy programming
//  }

//  static bool all(unsigned varble){
//    return (mask & varble) == mask;
//  }

//  static bool any(unsigned varble){
//    return mask & varble;
//  }

//  static bool none(unsigned varble){
//    return (mask & varble) == 0;
//  }

//};


//template <unsigned ... list> constexpr unsigned maskBuilder();

//template <unsigned pos> constexpr unsigned maskBuilder<pos>(){
//  return 1<<pos;
//}

//template <unsigned pos,unsigned... poss> constexpr unsigned maskBuilder<pos,poss>{
//  return maskBuilder<pos>() | maskBuilder<poss...>();
//}

#endif // BITBANGER_H
