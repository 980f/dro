#ifndef FEATURE_H
#define FEATURE_H

/** many internal things have a set of registers for control and status.
one register is 'set ones corresponding'
another register is 'clear ones corresponding'
a 3rd is a read back of the bits, and sometimes allows full write
a 4th is sometimes also bit oriented, others just loosely related to the others.
*/
namespace SAM {
class Feature {
  const unsigned base;
  const unsigned mask;
  /** the manual shows a total disregard for consistency in the relationship between the readback and the set/clear addresses */
  const bool invertedSense;
public:
  Feature(unsigned base, unsigned mask,bool invertedSense=false);
  void operator =(bool enableit) const;
  operator bool()const;
  //access to 4th member of group as independent value
  unsigned & fourth()const;
  /** set correlated bit in 4th component */
  void set4th(bool on);
  /** get correlated bit in 4th component */
  bool get4th();
};


void protectControlRegisters(bool on);

/** RIAA to ensure we leave controls locked when we aren't focussed on manipulating them.*/
class ExposeControls {
public:
  ExposeControls(){
    protectControlRegisters(false);
  }
  ~ExposeControls(){
    protectControlRegisters(true);
  }
};


}

#endif // FEATURE_H
