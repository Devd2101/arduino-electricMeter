#include <EEPROM.h>
//================================================================
//struct to meter reading variable 
struct Meter{
  int pulseCount;
  int mread;
};
//================================================================
struct Threshold {
  int th;
};
//================================================================
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("================================================");
  Serial.println( "Meter Data From EEPROM :" );
  int eeAddress = 0;
  Meter customVar = {0,1000};
  Threshold th ={1025};
  EEPROM.put(eeAddress, customVar);
  eeAddress += sizeof(MyObject);
  EEPROM.put(eeAddress, th);
  Serial.println("Successfully write the data.");
  }
//================================================================

void loop() {
  /* Empty loop */
}
//================================================================
