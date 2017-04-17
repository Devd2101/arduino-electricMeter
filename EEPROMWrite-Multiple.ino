
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

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("================================================");
  Serial.println( "Meter Data From EEPROM :" );
    int eeAddress = 0;
  Meter1 mr1 = {92};
  Meter2 mr2 = {92};
  Threshold  th1 = {25};
  EmPulse1 empulse1 = {200};
  EmPulse2 empulse2 = {200};
  EEPROM.put(eeAddress, mr1);
  eeAddress += sizeof(Meter1);
  EEPROM.put(eeAddress, mr2);
  eeAddress += sizeof(Meter2);
  EEPROM.put(eeAddress, th1);
  eeAddress += sizeof(Threshold);
  EEPROM.put(eeAddress, empulse1);
  eeAddress += sizeof(EmPulse1);
   EEPROM.put(eeAddress, empulse2);
   Serial.println("Successfully write the data.");
  
  }

void loop() {
  /* Empty loop */
}
