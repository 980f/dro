/**
  * Software Implemented I2C Master
  * Drive the I2C bus as a single master, 400kHz slaves that never stretch the clock.
  */
#include "i2cswmaster.h"
#include "minimath.h" //for nanoSpin

/**1MBaud EEPROM is the only thing we care to have work
  * start to first clock 260ns;
  * clock low: 500ns
  * clock high: 260ns
  * data setup (before clock rise) 50ns
  * clock edges: 120ns
  * stop to start: 500ns
  * read access:450ns
  * atmel adds: clock low to new data: 550ns , clock high 400ns!
  * st clock 300/400
  *
  * st setup: 5ns, slew ~5 as well.
  *
  */


#define nanoTicks(nanoseconds)  nanoSpin(nanoseconds / 13)

I2C::I2C(int luno, bool alt1){
  pinbase = 2 + 4 * luno + (alt1 ? 2 : 0);
}

void I2C::configurePins(const Port&sdaPort, int sdaPinNumber, const Port&sclPort, int sclPinNumber){
  Pin sdapin(sdaPort, sdaPinNumber);
  Pin sclpin(sclPort, sclPinNumber);
  sda = sdapin.DO(50, false); //will toggle pin function from normal to OD via sdaOff control
  *sda = 1;
  sdaOff = sdapin.highDriver();
  scl = sclpin.DO(50, false); //don't support slave clock stretching at high speed
  *scl = 1;
  sdaRead = sdapin.reader();
  sclRead = sclpin.reader();
} /* configurePins */

void I2C::configurePins(bool ){
  configurePins(PB, pinbase + 1, PB, pinbase);
}

/** clock and data should already be definitely high before calling this. */
void I2C::SendStart( void ){
  *scl = 1; //just in case...
  *sda = 1;
  *sdaOff = 0;
  *sda = 0; //1->0 while clock high == start
  nanoTicks(500);
  *scl = 0;
  nanoTicks(300); //st is sluggish, use 300 instead of 260
}
//deferred these defines to where we won't be tempted to corrupt them for special uses, like starts.
#define FloatSDA          *scl = 0; *sda = 1; *sdaOff = 1
#define DriveSDA(whatto)  * scl = 0; *sda = whatto; *sdaOff = 0

/** SendStop - sends an I2C stop, releasing the bus.
  * by leaving this a subroutine call we get the required deadtime between stop and start.
  */
void I2C::SendStop( void ){
  DriveSDA(0);
  *scl = 1;
  nanoTicks(500); //generous stop
  *sda = 1; //data 0->1 while clock high == stop.
  //single master: *sdaOff=1;
  nanoTicks(500); //guaranteed silence between transactions
}

/**
  * wiggleBits - sends one byte of data to an I2C slave device.
  * clock is left high, ack bit is still on the wire upon return.
  * @return the ack bit from the byte transfer.*/
bool I2C::wiggleBits( u8 A ){
  register unsigned bitPicker = 1 << 7;

  do {
    DriveSDA((A & bitPicker) ? 1 : 0);
    bitPicker >>= 1;
    nanoTicks(500); //clock low
    *scl = 1;
    nanoTicks(400); //atmel is sluggish
  } while(bitPicker);
  FloatSDA;  // Release data line for acknowledge.
  nanoTicks(500); //500 was coming out as 370!
  *scl = 1;
  nanoTicks(500); //gross delay to see if ack is just late.
  return *sdaRead; //Set status for no acknowledge.
} /* wiggleBits */

/**
  * on entrance sda is being driven
  * on exit sda is being driven to the ack value*/
u8 I2C::readBits(bool nackem){
  int BitCnt = 8;
  u8 A = 0; //assignment for debug

  FloatSDA;
  do {
    *scl = 0;
    nanoTicks(450); //read access time
    *scl = 1;
    A <<= 1;
    A |= *sdaRead;
    nanoTicks(400); //atmel is sluggish
  } while(--BitCnt > 0);
  DriveSDA(nackem);
  nanoTicks(400); //atmel is slow, should be 260
  *scl = 1;
  return A;
} /* readBits */



bool I2C::notify(bool happy){
  SendStop();
  if(inprogress) {
    inprogress->onTransferComplete(happy);
    inprogress = 0;
  }
  return happy;
}

bool I2C::transact(Transactor&transact){
  inprogress = &transact;
  bool justProbing = !inprogress->moreToWrite();
  justProbing &= !inprogress->moreToRead();
  if(justProbing) {
    SendStart();
    return notify(!wiggleBits(inprogress->device | 1));
  }
  if(inprogress->moreToWrite()) { //if something to write
    SendStart();
    if(wiggleBits(inprogress->device )) { //send address w/write selected
      //address not acknowledged
      return notify(false);
    }
    while(inprogress->moreToWrite()) {
      if(wiggleBits(inprogress->nextByte())) {
        //extremely rare to get here
        return notify(false);
      }
    }
    DriveSDA(1); //finish ack's clock and take back control of sda
    nanoTicks(250); //setup for repeated start.
  }
  if(inprogress->moreToRead()) { //is something to read
    SendStart(); //often a repeated start
    if(wiggleBits(inprogress->device | 1)) { //send address w/read selected
      //address not acknowledged
      return notify(false);
    }
    bool more; //a bit of nastiness just to NACK the last byte we read, a stupid part of the I2C spec since STOP already carries that info.
    do {
      u8 *target = &inprogress->nextByte();
      more = inprogress->moreToRead();
      *target = readBits(!more);
    } while(more);
    DriveSDA(1);
    nanoTicks(250); //setup for stop
  }
  return notify(true);
} /* transact */
//end of file
