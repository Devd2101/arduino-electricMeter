#include <EEPROM.h>
//================================================================
//struct to meter reading variable 
struct Meter {
  int pulseCount;
  int mread;
};
//================================================================
struct Threshold {
  int th;
};
//================================================================
void setup(){
  int eeAddress = 0;
  Serial.begin( 9600 );
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Meter customVar; //Variable to store custom object read from EEPROM.
  Threshold th ;
  EEPROM.get( eeAddress, customVar );
  Serial.println("================================================");
  Serial.println( "Meter Data Read From EEPROM :" );
   Serial.print("Pulse count : ");
  Serial.print( customVar.pulseCount);
  Serial.print("\tMeter reading : ");
  Serial.println( customVar.mread );
  eeAddress += sizeof(MyObject);
  EEPROM.get(eeAddress, th);
  Serial.print("threshold : ");
  Serial.println(th.th);
  Serial.println("================================================");
}
//================================================================
void loop(){ 
  /* Empty loop */ 
  }
  //================================================================
