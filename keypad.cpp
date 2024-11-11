/* labx2000 keypad and display project
 * this attempt is with lpc1343, using Olimex P1343 dev kit (8 LED's :)
 */

#include "gpio.h"
#include "lpc13xx.h"
#include "nvic.h"

//shared ~millisecond services:
#include "msservice.h"

CyclicTimer ledToggle(750); //todo: clearer timebase
RegisterTimer(ledToggle);   //a polled timer, a macro for 'declare and register' could be made but it would have to be variadic to allow constructor args.

#include "core_cmInstr.h" //wfe OR wfi
#include "p1343_board.h"  //P1343devkit //for led's
#include "wtf.h"

using namespace LPC;
const P1343devkit kit;

const OutputPin<2, 9> alePin;
const InputPin<2, 10> keyPin;
const OutputPin<2, 8> wePin;

GpioOutputField adb(2, 7, 0);
GpioField keycode(3, 3, 0);

const unsigned numAplha = 24;
//edge detector for 'keypressed'
bool kp;

void adbData(int octet) {
  adb = octet;
  //may need to nanospin here.
}

void select(int octet) {
  alePin = 0;;
  adbData(octet);
  alePin = 1;
}

void writeByte(unsigned luno, unsigned octet) {
  select(luno);
  wePin = 0;
  adbData(octet);
  wePin = 1;
}

void writeDigit(unsigned digit, unsigned segments) {
  writeByte(numAplha + (digit & 7), segments);
}

unsigned readKey() {
  unsigned code = 0;
  select(32);//enables keypad output
  select(32);//a second time to kill time.

  code = keycode;
  select(36);//this clears key present indicator, unless there is already another key.
  return code;
}

char ascii4code(unsigned keycode) {
  return "0123456789\nN-\r.Y"[keycode & 0xf];//4 bit code
}

bool keyPresent() {
  return keyPin;//so says the voltmeter.
}

//segments for display:
unsigned char numDisplay[8];

bool displayNeedsUpdate = true;

const unsigned char decimalSegments[] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6F, //empirically developed map of bit to segment
  0x77,//10: doesn't seem to happen, hiding under the Enter key?
  0x7C,//11: No
  0x39,//12: -
  0x5e,//13: \r
  0x79,//14: '.'
  0x71 //15: Yes
};

const char oh = 0x5C;
const char arr = 0x50;
const char blank = 0;
const char capE = 0x79;

const char faceoff[] = {
  0x71, 0x77, 0x39, 0x79, 0x00, 0x3f, 0x71, 0x71};
const char deadbeef[] = {
  0x5e, 0x79, 0x77, 0x5e, 0x7C, 0x79, 0x79, 0x71};

void clearNumber() {
  for (int digit = 8; digit-- > 0;) {
    numDisplay[digit] = 0;
  }
  displayNeedsUpdate = true;
}

char alphaDisplay[numAplha];

void refreshAlpha() {
  for (unsigned di = numAplha; di-- > 0;) {
    writeByte(di, alphaDisplay[di]);
  }
}

void updateAlpha(const char *text) {
  for (unsigned ai = 0; *text && ai < numAplha; ++ai) {
    alphaDisplay[ai] = *text++;
  }
  refreshAlpha();
}

void refreshNumDisplay() {
  for (unsigned digit = 8; digit-- > 0;) {
    writeDigit(digit, numDisplay[digit]);
  }
  select(36);
  displayNeedsUpdate = false;
}

void shiftNum(unsigned code) {
  if (code == 14) {//decimal point
    numDisplay[0] |= 0x80;
  } else {
    for (unsigned digit = 8; digit-- > 1;) {
      numDisplay[digit] = numDisplay[digit - 1];
    }
    numDisplay[0] = decimalSegments[code];
  }
  displayNeedsUpdate = true;
}

void numBackspace(unsigned segments) {
  for (unsigned digit = 0; digit < 7; digit++) {
    numDisplay[digit] = numDisplay[digit + 1];
  }
  numDisplay[7] = segments;
  displayNeedsUpdate = true;
}

void numString(const char *segmented) {
  for (unsigned digit = 8; digit-- > 0;) {
    numDisplay[digit] = *segmented++;
  }
  displayNeedsUpdate = true;
}

bool waker;

void setup() {
//  Host.begin(57600);
//
//  forAdb{
//    pinMode(pin, OUTPUT);      
//  }
//  pinMode(alePin,OUTPUT);
//  pinMode(wePin,OUTPUT);
//
//  pinMode(keyPin,INPUT);
//
//  for(int pin=keyPin;pin -->keyPin-4;){
//    pinMode(pin,INPUT);
//  }
  kp = keyPresent();
  waker = kit.wakeup;
//  clearNumber();
  numString(deadbeef);
  refreshNumDisplay();

  updateAlpha("Pathogentech ZOMBIE");
}

int numdistoggle = 0;

void loop() {

  if (changed(kp, keyPresent())) {
    if (kp) {
      int code = readKey();
//      echo=ascii4code(code);
//      Host.write(echo);
      switch (code) {
      default: //0-9,.
      case 14://decimal point key
        shiftNum(code);
        break;

      case 10://may be the other key under the ENTER button.
      case 13://ENTER
        clearNumber();
        break;
      case 12://- backspace
        numBackspace(0);
        break;
      case 11://no
        numString(deadbeef);
        break;
      case 15://yes
        numString(faceoff);
        break;
        //if ENTER then do something nifty, then set flag to wipe display on next stroke.
      }
    }
  }
  if (displayNeedsUpdate) {
    refreshNumDisplay();
  }
}

int main(void) {

  startPeriodicTimer(1000); //shoot for millisecond resolution
  setup();//arduino heritage ;)


#pragma ide diagnostic ignored "EndlessLoop"
  while (true) {
    //the WFE below wakes up at least every millisecond due to the systick being programmed for 1kHz.
    MNE(WFE); //WFE is more inclusive than WFI, events don't call an isr but do wakeup the core.
    if (ledToggle) {
      numString(numdistoggle ? deadbeef : faceoff);
      refreshNumDisplay();
      numdistoggle = 1 - numdistoggle;
    }

    if (changed(waker, bool(kit.wakeup))) {
      numString(waker ? deadbeef : faceoff);
      refreshNumDisplay();
    }

    loop();
  }
#pragma ide diagnostic ignored "UnreachableCode"
  return 0;
}

void SystemInit() {
  GPIO::Init();
}
