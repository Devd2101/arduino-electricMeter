#include <EEPROM.h>
#include <stdio.h>
#include <DS1302.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <string.h>

//=========================================================
SoftwareSerial mySerial(5, 6);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 12, 11, 10, 9);
//=========================================================
String response = "";
char data[64];
int count = 0;
String res = "";
int power = 4;
//=========================================================
struct Meter1 {
  int mread;
};
//=========================================================
struct Meter2 {
  int mread;
};
//=========================================================
struct Threshold {
  int th;
};
//=========================================================
struct EmPulse1 {
  int pulseCount;
};
//=========================================================
struct EmPulse2 {
  int pulseCount;
};
//=========================================================
const byte ledPin = 13;
const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
volatile byte state = LOW;
int pulseCount1,pulseCount2, mread1,mread2, th1;
int eeAddress = 0;
//=========================================================
void setup() {
      powerUp();
      delay(1000);
      Serial.begin(9600);
      mySerial.begin(19200);
      lcd.begin(16, 2);
      lcd.setCursor(0, 0); // set up the LCD's number of columns and rows:
      lcd.print(" Electric Meter ");// Print a message to the LCD.
      //=====================================================================
      int eeAddress = 0;
      Meter1 mr1; //Variable to store custom object read from EEPROM.
      Meter2 mr2;
      EmPulse1 empulse1 ;
      EmPulse2 empulse2;
      Threshold Th ;
     
      Serial.println("=========================================================");
      Serial.println("Initialize the EEPROM");
       EEPROM.get(eeAddress, mr1);
      mread1 = mr1.mread;
      Serial.print("Meter Reading1 = ");
      Serial.println(mread1);
      //=====================================================================
      eeAddress += sizeof(Meter1);
      EEPROM.get(eeAddress, mr2);
      mread2 = mr2.mread;
      Serial.print("Meter Reading2 = ");
      Serial.println(mread2);
      //===================================================================== 
      eeAddress += sizeof(Meter2);
      EEPROM.get(eeAddress, Th);
      th1 = Th.th;
      Serial.print("Threshold = ");
      Serial.println(th1);
      //=====================================================================
      eeAddress += sizeof(Threshold);
      EEPROM.get(eeAddress, empulse1);
      pulseCount1 = empulse1.pulseCount;
      Serial.print("PulseCount = ");
      Serial.println(pulseCount1);
      //===================================================================== 
      eeAddress += sizeof(EmPulse1);
      EEPROM.get(eeAddress, empulse2);
      pulseCount2 = empulse2.pulseCount;
      Serial.print("PulseCount = ");
      Serial.println(pulseCount2);
      //===================================================================== 
      pinMode(ledPin, OUTPUT);
      //pinMode(3, OUTPUT);
      //pinMode(interruptPin, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(interruptPin1), blink, FALLING);
      attachInterrupt(digitalPinToInterrupt(interruptPin2), blink1, FALLING);
      //=====================================================================
      Serial.println("=========================================================");
      mySerial.begin(19200);    // change to your baud rate, if not auto-baud
      mySerial.println("AT+CMGF=1\r");
      delay(1000);
      mySerial.println("AT+CNMI=2,2,0,0,0");
      delay(1000);

}
//=========================================================
void loop() {
    //mySerial.println("AT+CNMI=2,2,0,0,0");
    if (mySerial.available()) {
      while (mySerial.available()) {
        data[count++] = mySerial.read();
        if (count == 64)break;
      }
      //Serial.print(data);
      response = getData();
      Serial.println(response);
      delay(100);
      res = "";
      response = "";
    }
    if (Serial.available() > 0) {
      mySerial.write(Serial.read());
    }
    //digitalWrite(ledPin, state);
    //digitalWrite(3, HIGH);
    Serial.println("=========================================================");
    Serial.print("PulseCount1 = ");
    Serial.print(pulseCount1);
    Serial.print("\tPulseCount2 = ");
    Serial.println(pulseCount2);
    Serial.print("Meter reading1 = ");
    Serial.print(mread1);
    Serial.print("\tMeter reading2 = ");
    Serial.println(mread2);
    Serial.println("=========================================================");
    //=====================================================================
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pulse 1 = ");
    lcd.print(pulseCount1);
    lcd.setCursor(0, 1);
    lcd.print("Reading = ");
    lcd.print(mread1);
    delay(3000);
    //=====================================================================
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pulse 2 = ");
    lcd.print(pulseCount2);
    lcd.setCursor(0, 1);
    lcd.print("Reading = ");
    lcd.print(mread2);
    delay(3000);
    //=====================================================================
    if (pulseCount1 >= 3200) {
      mread1++;
      Serial.println(mread1);
      delay(20);
      pulseCount1 = 0;
      writePulse1(pulseCount1);
      writeMr1(mread1);
    }
    //=====================================================================
    if (pulseCount2 >= 3200) {
      mread2++;
      Serial.println(mread2);
      delay(20);
      pulseCount2 = 0;
      writePulse2(pulseCount2);
      writeMr2(mread2);
    }
    //=====================================================================
    if (mread1 > th1) {
      th1 = th1 + 25;
      writeTh(th1);
      SendTextMessage("+919165507806",mread1,mread2);
      lcd.clear();
      lcd.print("Threshold = ");
      lcd.print(th1);
      delay(2000);
    }
    //=====================================================================
     
    //digitalWrite(3, LOW);
    //delay(1000);
 }

//=========================================================
void blink() {
  pulseCount1++;
  writePulse1(pulseCount1);
}
//=========================================================
void blink1() {
  pulseCount2++;
  writePulse2(pulseCount2);
}
//=========================================================
void writeMr1(int mread) {
  int eeAddress = 0;
  Meter1 mr = {mread};
  EEPROM.put(eeAddress, mr);
}
//=========================================================
void writeMr2(int mread) {
  int eeAddress = 0;
  eeAddress += sizeof(Meter1);
  Meter2 mr = {mread};
  EEPROM.put(eeAddress, mr);
}
//=========================================================
void writeTh(int th) {
  int eeAddress = 0;
  eeAddress += sizeof(Meter1);
  eeAddress += sizeof(Meter2);
  Threshold Th = {th};
  EEPROM.put(eeAddress, Th);
}
//=========================================================
void writePulse1(int p) {
  int eeAddress = 0;
  eeAddress += sizeof(Meter1);
  eeAddress += sizeof(Meter2);
  eeAddress += sizeof(Threshold);
  EEPROM.put(eeAddress, p);
}
//=========================================================
void writePulse2(int p) {
  int eeAddress = 0;
  eeAddress += sizeof(Meter1);
  eeAddress += sizeof(Meter2);
  eeAddress += sizeof(Threshold);
  eeAddress += sizeof(EmPulse1);
  EEPROM.put(eeAddress, p);
}
//=========================================================
void SendTextMessage(String mobile,int m1,int m2)
{
  String msg1 = "Meter-1 Reading : " + String(m1) +"\nMeter-2 Reading : " + String(m2);
  String SMSsenderString = "AT+CMGS=\"" + mobile + "\"\r";
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println(String(SMSsenderString));
  delay(1000);
  mySerial.println(String(msg1));// The SMS text you want to send
  delay(500);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Msg send successfully...");
}
//=========================================================
String getData()           // function to clear buffer array
{
  String mob = "";
  String res = "";
  for (int i = 0; i < count; i++)
  {
    res.concat(data[i]);
    data[i] = NULL;
  }
  delay(10);
  res.trim();
  if (res.startsWith("+CMT:")) {
    mob = "";
    mob = res.substring(7, 20);
    Serial.println(mob);
    SendTextMessage(String(mob),mread1,mread2);
  }
  else if (res.startsWith("RING")) {
    mySerial.println("ATH\r");
    int pos = find_text("+91", res);
    String mob = res.substring(pos, pos + 13);
    Serial.print(mob);
    SendTextMessage(String(mob),mread1,mread2);
    delay(100);
  }
  count = 0;
  return mob;
}
//=========================================================
int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i, needle.length() + i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
//=========================================================
void powerUp() {
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(power, LOW);    // turn the LED off by making the voltage LOW              // wait for a second
}
//=========================================================
/*
  namespace {

  //   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
  const int kCePin   = 12;  // Chip Enable
  const int kIoPin   = 11;  // Input/Output
  const int kSclkPin = 10;  // Serial Clock
  // Create a DS1302 object.
  DS1302 rtc(kCePin, kIoPin, kSclkPin);

  String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
  }

  void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
  }
  }

  void timeDisplay()
  {
    Time t = rtc.time();
    // Name the day of the week.
    const String day = dayAsString(t.day);
    char buf1[15];
    char buf2[20];
    snprintf(buf1, sizeof(buf1), "%04d-%02d-%02d",t.yr, t.mon, t.date);

    snprintf(buf2, sizeof(buf2), "%s %02d:%02d",day.c_str(),t.hr, t.min);

    // Print the formatted string to serial so we can see the time.
    //Serial.println(buf1);
    //Serial.println(buf2);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print(buf1);
    lcd.setCursor(0,1);
    lcd.print(buf2);
    delay(1000);
  }
*/
