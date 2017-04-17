#include <EEPROM.h>
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 12, 11, 10, 9);
//================================================================
//struct to meter reading variable 
struct Meter {
  int pulseCount;
  int mread;
};
//================================================================
//define global variable
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
int pulseCount1, mread1;
int eeAddress = 0;
//================================================================
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.print(" Electric Meter ");// Print a message to the LCD.
  int eeAddress = 0;
  Meter customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get(eeAddress, customVar);
  Serial.println("================================================");
  Serial.println("Meter data From EEPROM : ");
  pulseCount1 = customVar.pulseCount;
  Serial.print(pulseCount1);
  Serial.print("\t");
  mread1 = customVar.mread;
  Serial.println(mread1);
  Serial.println("================================================");
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}
//================================================================
void loop() {
  digitalWrite(ledPin, state);
  Serial.print(pulseCount1);
  Serial.print("\t");
  Serial.println(mread1);
  Serial.println("================================================");
  lcd.clear(); // clear the content on lcd screen
  lcd.setCursor(0,0);//set the cursor at first row,first column
  lcd.print("PulseCount= ");
  lcd.print(pulseCount1);
  lcd.setCursor(0, 1);//set the cursor at second row,first column
  lcd.print("mread = ");
  lcd.print(mread1);
  delay(2000);
  if (pulseCount1 >= 100) {
    mread1++;
    Serial.println(mread1);
    delay(5000);
    pulseCount1 = 0;
    Meter customVar = {pulseCount1, mread1};
    EEPROM.put(eeAddress, customVar);
  }
}
//================================================================
// ISR to count the incomming pulse from electric Meter
void blink() {
  state = !state;
  pulseCount1++;
  writeVal(pulseCount1, mread1);
}
//================================================================
// write the value of meter pulse and meter reading in eeprom
void writeVal(int pulseCount, int mread) {
  int eeAddress = 0;
  Meter customVar = {pulseCount, mread};
  EEPROM.put(eeAddress, customVar);
}
//================================================================


