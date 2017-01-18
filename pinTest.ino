
/* @param mode shoudl be one of:: INPUT, INPUT_PULLUP, and OUTPUT */
template <unsigned arduinoNumber,unsigned mode,unsigned polarity=digitalHigh> class Pin {
public:
  Pin(){
    pinMode(arduinoNumber,mode);
  }
  volatile operator bool()const{
    return digitalRead(arduinoNumber)==polarity;
  }
  bool operator =(bool value){
    digitalWrite(arduinoNumber,value?polarity:digitalHigh+digitalLow-polarity);
  }
}

/** pin 13 is attached to the high side of an LED */
Pin<13,0,digitalHigh> LED;//static constructors run just before main() is called, which in turn calls setup and loop.
Pin(12,1,digitalLow> button;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  LED=!button;
}
