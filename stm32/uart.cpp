#include "uart.h"
#include "minimath.h"
#include "afio.h"
void Uart::setBaudrate(unsigned int desired){
  //note: the ST manuals are chocked full of nonsense with respect to baud rate setting.
  // just take the input clock rate and divide by the baud rate, round to the nearest integer, and put that value into the 16 bit BRR as if it were a simple integer.
  // you can ignore all the jabber about fractional baudrate and all the dicing and splicing, which under close inspection of ST's code does absolutely nothing.
  unsigned int osc = apb.getClockRate();
  unsigned int newbaud = rate(osc, desired);

  if(newbaud != dcb->BRR) {
    b->enable = 0;
    dcb->BRR = newbaud;
  }
} /* setBaudrate */


u32 Uart::bitsPerSecond() const {
  u32 divisor = dcb->BRR;
  unsigned int osc = apb.getClockRate();

  return rate(osc, divisor);
}

/**
  * st's manual fails to note that 8 data bits + parity bit means you must set the word length to 9!
  * So: 9 if sending 9 bits and no parity or 8 bits with parity. Set to 8 for 7 bits with parity.
  */
void Uart::setParams(unsigned int baud, unsigned int numbits, char parityNEO, bool longStop, bool shortStop){ //19200,8,n,1
  b->enable = 0;
  //decode char to the control bits:
  b->parityOdd = parityNEO & (1 << 1); //bit 1 is high for Oh versus low for E
  b->parityEnable = parityNEO & 1; ////lsb is 1 for E or Oh, 0 for N.
  b->_9bits = numbits == 9 || (numbits == 8 && b->parityEnable);
  b->halfStop = shortStop;
  b->doubleStop = longStop;
  setBaudrate(baud);
}

/** part of char time calculation, includes stop and start and parity, not just payload bits */
u32 Uart::bitsPerByte(void) const {
  u32 bits = 1; //the start bit

  if(b->doubleStop) {
    bits += 2;
  }
  if(b->halfStop) { //sorry, not rigged to deal with 1.5 stop bits,
    bits -= 0; //should be decreasing by 0.5 bits.
  }
  if(b->_9bits) {
    return bits + 9;
  }
  return bits + 8; //todo:2 7 bits no parity!
} /* bitsPerByte */

/** timer ticks required to move the given number of chars. Involves numbits and baud etc.*/
u32 Uart::ticksForChars(unsigned charcount) const {
  return charcount * bitsPerByte() * dcb->BRR;
}

void Uart::reconfigure(unsigned int baud, unsigned int numbits, char parityNEO, bool longStop, bool shortStop){ //19200,8,n,1
  apb.init();
  setParams(baud, numbits, parityNEO, longStop, shortStop);
}

Uart::Uart(unsigned int zluno, unsigned int alt): apb(zluno ? 1: 2, zluno ? (zluno + 16): 14), irq((zluno < 3 ? 37: 49) + zluno), zluno(zluno), altpins(alt){
  dcb = reinterpret_cast <USART_DCB *> (apb.getAddress());
  b = reinterpret_cast <UartBand *> (apb.getBand());
  //not grabbing pins quite yet as we may be using a spare uart internally as a funky timer.
}

/**
rx's are coded as floating inputs as RX is presumed to always be driven
hsin's are coded as pulled up inputs in case the pin is left unconnected, although really someone
should actively configure the pin's presence and not rely upon our weak pullup.
*/
//got tired of finesse:
#define makeTxPin(P,b) Pin(P, b).FN(rxtxSpeedRange)

#define pinMux(stnum) theAfioManager.field.uart##stnum=altpins;theAfioManager.update()

void Uart::takePins(bool tx, bool rx, bool hsout, bool hsin){
  int rxtxSpeedRange=bitsPerSecond()>460e3?10:2;
  switch(zluno) {
  case 0: //st's 1
    pinMux(1);
    if(hsin) {
      Pin(PA, 11).DI('U');
    }
    if(hsout) {
      Pin(PA, 12).FN();
    }
    switch(altpins) {
    case 0 :
      if(tx) {
        makeTxPin(PA, 9);
      }
      if(rx) {
        Pin(PA, 10).DI('F');
      }
      break;
    case 1 :
      if(tx) {
        makeTxPin(PB, 6);
      }
      if(rx) {
        Pin(PB, 7).DI('F');
      }
      break;
    } /* switch */
    break; //todo:M CHECK XRT! this break was missing which caused pins A2,A3 to be taken whenever A9,A10 were.
  case 1: //st's 2
    pinMux(2);
    switch(altpins) {
    case 0:
      if(tx) {
        makeTxPin(PA, 2);
      }
      if(rx) {
        Pin(PA, 3).DI('F');
      }
      if(hsin) {
        Pin(PA, 0).DI('U');
      }
      if(hsout) {
        Pin(PA, 1).FN();
      }
      break;
    case 1:
      //todo:3 set the AFIO remap field
      if(tx) {
        makeTxPin(PD, 5);
      }
      if(rx) {
        Pin(PD, 6).DI('F');
      }
      if(hsin) {
        Pin(PD, 3).DI('U');
      }
      if(hsout) {
        Pin(PD, 4).FN();
      }
      break;
    } /* switch */
    break;
  case 2: //uart 3
    pinMux(3);
    //todo:3 add hs lines
    switch(altpins) {
    case 0:
      if(tx) {
        makeTxPin(PB, 10);
      }
      if(rx) {
        Pin(PB, 11).DI('F');
      }
      break;
    case 1:
      if(tx) {
        makeTxPin(PC, 10);
      }
      if(rx) {
        Pin(PC, 11).DI('F');
      }
      break;
    case 3:
      if(tx) {
        makeTxPin(PD, 8);
      }
      if(rx) {
        Pin(PD, 9).DI('F');
      }
      break;
    } /* switch */
    break;
    //todo:3 uarts 4 and 5, which don't suffer from remap options.
  } /* switch */
  b->enableTransmitter = tx; //else the tx pin floats!
  //... would only dynamically play with this for single wire half duplexing as in some SPI modes, better to use a transceiver and
  //... a gpio pin than to play with the chip's own pin.
} /* takePins */
//End of file
