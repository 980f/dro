/*
sam3x has 3 modules each of which has 3 timers.
The 9 timers are nearly independent, the only interaction is that you can route some of the outputs of one of a module to the inputs of another of the same module.

First pass we will only try to count external inputs.

*/
class CountPin {
  /** an integer 0 to 2 */
  const unsigned tcu;
  const unsigned tmod;
  const unsigned base;

public:
  CountPin(unsigned tc1of9, unsigned arduinoPinNumber);
  void configure(unsigned arduinoPinNumber)const;
  void start(bool withReset=true)const;
  void stop()const;
  /** read the count value */
  operator unsigned()const;
  /** read, reset and restart */
  unsigned roll()const;
};

