/*
 * P1343devkit.h
 *
 *  Created on: Apr 14, 2014
 *      Author: andyh
 */

#ifndef P1343DEVKIT_H_
#define P1343DEVKIT_H_

#include "gpio.h" //to declare buttons and lamps.
namespace LPC {
class P1343devkit {
public:
  InputPin<2,9> button;
  OutputPin<3,0> led0;
  OutputPin<3,1> led1;
  OutputPin<3,2> led2;
  OutputPin<3,3> led3;
  OutputPin<2,4> led4;
  OutputPin<2,5> led5;
  OutputPin<2,6> led6;
  OutputPin<2,7> led7;
  P1343devkit();
  virtual ~P1343devkit();
  /** set lamps as an 8-bit number, not particular swift in execution since they are scattered about the i/o space*/
  int operator =(int lamp );
};
}

#endif /* P1343DEVKIT_H_ */
