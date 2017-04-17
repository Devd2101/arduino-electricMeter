
#include <EEPROM.h>

struct Meter1 {
  int mread;
};
struct Meter2 {
  int mread;
};

struct Threshold {
  int th;
};
struct EmPulse1 {
  int pulseCount;
};
struct EmPulse2 {
  int pulseCount;
};

void setup() {
  int eeAddress = 0;
  Serial.begin( 9600 );
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print( "Read float from EEPROM: " );

  Meter1 mr1; //Variable to store custom object read from EEPROM.
  Meter2 mr2;
  EmPulse1 empulse1 ;
  EmPulse2 empulse2;
  Threshold th1;
  
  EEPROM.get( eeAddress,mr1);
  Serial.println( mr1.mread );
  
  eeAddress += sizeof(Meter1);
  EEPROM.get( eeAddress, mr2 );
  Serial.println(mr2.mread);
  
  eeAddress += sizeof(Meter2);
  EEPROM.get( eeAddress, th1 );
  Serial.println(th1.th);
  
  eeAddress += sizeof(Threshold);
  EEPROM.get( eeAddress,empulse1 );
  Serial.println( empulse1.pulseCount );

  eeAddress += sizeof(EmPulse1);
  EEPROM.get( eeAddress,empulse2 );
  Serial.println( empulse2.pulseCount );

}

void loop() {
  /* Empty loop */
}
