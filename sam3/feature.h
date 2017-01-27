#ifndef FEATURE_H
#define FEATURE_H

/** many internal things have a set of registers for control and status */
namespace SAM {
class Feature {
  const unsigned base;
  const unsigned mask;
  /** the manual shows a toital disregard for consistency in the relationship between the readback and the set/clear addresses */
  const bool invertedSense;
public:
  Feature(unsigned base, unsigned mask,bool invertedSense);
  void operator =(bool enableit) const;
  operator bool()const;
  //todo: access to 4th member of group, which only rarely exists.
};
}

#endif // FEATURE_H
