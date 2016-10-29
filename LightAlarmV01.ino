/*
  LightAlarm V01

  This example code is in the public domain.

  modified 8 May 2014
  by SVO
 */

#include <Time.h>
#include <TimeAlarms.h>

//=========================================== Clcok Startnt gRedChanel   = 10;
int gRedChanel   = 10;
int gGreenChanel = 11;
int gBlueChanel  = 12;

//=========================================== Clcok Start
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, 
                   byte minute, 
                   byte hour, 
                   byte dayOfWeek, 
                   byte dayOfMonth, 
                   byte month, 
                   byte year)
{
    Serial.println("Set time");
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    
    // request seven bytes of data from DS3231 starting from register 00h
    *second     = bcdToDec(Wire.read() & 0x7f);
    *minute     = bcdToDec(Wire.read());
    *hour       = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek  = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month      = bcdToDec(Wire.read());
    *year       = bcdToDec(Wire.read());
}

void initiateTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  //Time
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  setTime(23,34,50,04,10,2016);
  //setTime(hour,minute,second,dayOfMonth,month,year);
}

void readTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  //Time
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);


  Serial.print(hour, DEC);
  printDigits(minute);
  printDigits(second);

//  Serial.print( '  ' + day() + '/' + month() + '/' + year() );
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC); 
  Serial.print("/");
  Serial.println(year, DEC);
}
//=========================================== Clock End


//=========================================== Alarm Start

void initiateAlarm() {
  Alarm.alarmRepeat(7,00,0, MorningAlarmStart); // 8:30am every day
  Alarm.alarmRepeat(7,30,0, WakeUpAlarmStart);  // 8:30am every day
  Alarm.alarmRepeat(7,45,0, GoAlarmStart);      // 8:30am every day
  Alarm.alarmRepeat(23,35,0,EveningAlarm);  // 5:45pm every day 
  //Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday 

 
  //Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
  //Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 
}

// functions to be called when an alarm triggers: MorningAlarmStart
void MorningAlarmStart(){
  Serial.println("Alarm: - MorningAlarmStart");
  digitalWrite(gRedChanel,    HIGH);    
}

// functions to be called when an alarm triggers: WakeUpAlarmStart
void WakeUpAlarmStart(){
  Serial.println("Alarm: - WakeUpAlarmStart");
  digitalWrite(gGreenChanel,    HIGH);    
}

// functions to be called when an alarm triggers: GoAlarmStart
void GoAlarmStart(){
  Serial.println("Alarm: - GoAlarmStart");
  digitalWrite(gBlueChanel,    HIGH);     
}

// functions to be called when an alarm triggers: EveningAlarm
void EveningAlarm(){
  Serial.println("Alarm: - EveningAlarm"); 

  digitalWrite(gRedChanel,    HIGH);
  digitalWrite(gGreenChanel,  LOW);
  digitalWrite(gBlueChanel,   LOW);

  delay(1000);

    digitalWrite(gRedChanel,    HIGH);
  digitalWrite(gGreenChanel,  HIGH);
  digitalWrite(gBlueChanel,   LOW);

  delay(1000);

    digitalWrite(gRedChanel,    HIGH);
  digitalWrite(gGreenChanel,  HIGH);
  digitalWrite(gBlueChanel,   HIGH);

    delay(1000);

    digitalWrite(gRedChanel,    LOW);
  digitalWrite(gGreenChanel,  HIGH);
  digitalWrite(gBlueChanel,   HIGH);
  
    delay(1000);

    digitalWrite(gRedChanel,    HIGH);
  digitalWrite(gGreenChanel,  HIGH);
  digitalWrite(gBlueChanel,   LOW);
  
    delay(1000);

    digitalWrite(gRedChanel,    HIGH);
  digitalWrite(gGreenChanel,  LOW);
  digitalWrite(gBlueChanel,   HIGH);

}


//=========================================== Alarm End


//=========================================== IR sensor ++ 
#include <IRremote.h>
#include <IRremoteInt.h>
//#include <ir_Lego_PF_BitStreamEncoder.h>
//
int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;


//=========================================== IR sensor --










//
//                    SET UP
//
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  //RGN initiate
  pinMode(gRedChanel,   OUTPUT);
  pinMode(gGreenChanel, OUTPUT);
  pinMode(gBlueChanel,  OUTPUT);

  setDS3231time(00,
                00,
                00,
                00,
                27,
                10,
                16);

  //Timer
  initiateTime();
  initiateAlarm();

  //IR
  irrecv.enableIRIn(); // Start the receiver  
}









//
//                    LOOP
//
// the loop function runs over and over again forever
void loop() {
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(500);              // wait for a second
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);
// wait for a second
  bool lRedChanelState   = false;
  bool lGreenChanelState = false;
  bool lBlueChanelState  = false; 

  digitalWrite(gRedChanel,    LOW);
  digitalWrite(gGreenChanel,  LOW);
  digitalWrite(gBlueChanel,   LOW);
  
  
  //IR read signal and aoutput to Serial
  if (irrecv.decode(&results)) {
    String value = String(results.value);

    if (value = "3622325019") {
      Serial.print("Pult signal: On - ");
    } else if(value = "3622325013") {
      Serial.print("Pult signal: Red - ");
    } else{
      Serial.print("Pult signal: Unknown - ");
    }

    Serial.println(value);
      
    Serial.print("HEX value: ");
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  

  //Time
  Alarm.delay(1000);        // wait one second between clock display
//  Serial.println("Cikl");
  Serial.print("Library time:");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());

//  Serial.print( '  ' + day() + '/' + month() + '/' + year() );
  Serial.print(" ");
  Serial.print(day());
  Serial.print('/');
  Serial.print(month()); 
  Serial.print('/');
  Serial.println(year());

  switch (Serial.read()) {
    case 'B':
      //do something when var equals B
      Serial.println("Command: B");
      digitalWrite(gBlueChanel,    HIGH);
      break;
    case 'R':
      //do something when var equals R
      Serial.println("Command: R");
      digitalWrite(gRedChanel,    HIGH);
      break;
    case 'G':
      //do something when var equals G
      Serial.println("Command: G");
      digitalWrite(gGreenChanel,    HIGH);
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

delay(1000);

//  if (Serial.read() == 'B') {
//     Serial.println("Command: B");
//     digitalWrite(gBlueChanel,    HIGH);
//     delay(1000);
//  }
//
//  if (Serial.read() == 'R') {
//     Serial.println("Command: R");
//     digitalWrite(gRedChanel,    HIGH);
//     delay(1000);
//  }
//
//  if (Serial.read() == 'G') {
//     Serial.println("Command: G");
//     digitalWrite(gGreenChanel,    HIGH);
//     delay(1000);
//  }

  // look for the newline. That's the end of your
  // sentence:
  if (Serial.read() == '\n') {
    //Serial.println("Sentence");
    Serial.print("Module time: ");
    readTime();
  }
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits, DEC);
}
