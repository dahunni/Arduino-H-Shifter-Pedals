#include <Keypad.h>

// Simple gamepad example that demonstraits how to read five Arduino
// digital pins and map them to the Arduino Joystick library.
//
// The digital pins 2 - 6 are grounded when they are pressed.
// Pin 2 = UP
// Pin 3 = RIGHT
// Pin 4 = DOWN
// Pin 5 = LEFT
// Pin 6 = FIRE
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2016-11-24
//--------------------------------------------------------------------
#include <Arduino.h>
#include <Joystick.h>
//gangschaltung

const byte rows = 4; //four rows
const byte cols = 2; //two columns
char keys[rows][cols] = {
  {'2','1'},
  {'4','3'},
  {'6','5'},
  {'8','7'}
};
byte rowPins[rows] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 4}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );




//pedale



int gas = 0;
int gasWert = 0;
int alterGas = 0;

int bremse = 0;
int bremseWert = 0;
int alterBremse = 0;

int kupplung = 0;
int kupplungWert = 0;
int alterKupplung = 0;

//mapping vals
int gasFromMap[] = {150, 159, 161, 167, 171, 178, 191, 230, 410, 950, 1023};
int gasToMap[] = {0, 1023};
byte gasFromMapSize;
byte gasToMapSize;

int bremseFromMap[] = {150, 215, 370, 517, 595, 700, 865, 966, 1023};
int bremseToMap[] = {0, 1023};
byte bremseFromMapSize;
byte bremseToMapSize;

int kupplungFromMap[] = {165, 217, 300, 500, 591, 680, 790, 870, 917, 990};
int kupplungToMap[] = {0, 1023};
byte kupplungFromMapSize;
byte kupplungToMapSize;


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  8, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  true, true, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

void setup() {

  //mapping stuff
gasFromMapSize = sizeof(gasFromMap) / sizeof(int);
gasToMapSize = sizeof(gasToMap) / sizeof(int);
bremseFromMapSize = sizeof(bremseFromMap) / sizeof(int);
bremseToMapSize = sizeof(bremseToMap) / sizeof(int);
kupplungFromMapSize = sizeof(kupplungFromMap) / sizeof(int);
kupplungToMapSize = sizeof(kupplungToMap) / sizeof(int);
  
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 1023);
}

// Last state of the buttons
int lastButtonState[5] = {0,0,0,0,0};

void loop() {
  char key = keypad.getKey();
  int lastkey = key - '0';

if (keypad.getState() == RELEASED)
{

    Joystick.releaseButton(0);
    Joystick.releaseButton(1);
    Joystick.releaseButton(2);
    Joystick.releaseButton(3);
    Joystick.releaseButton(4);
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    
}
  if(key != NO_KEY)
  {
    Joystick.pressButton(lastkey - 1);
    Joystick.releaseButton(lastkey - 2);
    Joystick.releaseButton(lastkey - 3);
    Joystick.releaseButton(lastkey - 4);
    Joystick.releaseButton(lastkey - 5);
    Joystick.releaseButton(lastkey - 6);
    Joystick.releaseButton(lastkey - 7);
    Joystick.releaseButton(lastkey - 8);

    Joystick.releaseButton(lastkey);
    Joystick.releaseButton(lastkey + 1);
    Joystick.releaseButton(lastkey + 2);
    Joystick.releaseButton(lastkey + 3);
    Joystick.releaseButton(lastkey + 4);
    Joystick.releaseButton(lastkey + 5);
    Joystick.releaseButton(lastkey + 6);
  }
  
  gas = analogRead(A2);
  bremse = analogRead(A1);
  kupplung = analogRead(A0);
 
 
delay(2);
 gasWert = multiMap(gas, gasFromMap, gasFromMapSize, gasToMap, gasToMapSize);
 delay(2);
 bremseWert = multiMbp (bremse, bremseFromMap, bremseFromMapSize, bremseToMap, bremseToMapSize);
 delay(2);
 kupplungWert = multiMcp (map(kupplung, 1024, 0, 0, 1024), kupplungFromMap, kupplungFromMapSize, kupplungToMap, kupplungToMapSize);
 delay(2);

Joystick.setXAxis(gasWert);
Joystick.setRyAxis(bremseWert);
Joystick.setRxAxis(kupplungWert);
 

}
int multiMap(int value, int fromMap[], int fromMapSize, int toMap[], int toMapSize)
{
  //Boundary cases
  if (value <= fromMap[0]) return toMap[0];
  if (value >= fromMap[fromMapSize - 1]) return toMap[toMapSize - 1];

  //Find the fromMap interval that value lies in
  byte fromInterval = 0;
  while (value > fromMap[fromInterval + 1])
    fromInterval++;

  //Find the percentage of the interval that value lies in
  float fromIntervalPercentage = (float)(value - fromMap[fromInterval]) / (fromMap[fromInterval + 1] - fromMap[fromInterval]);

  //Map it to the toMap interval and percentage of that interval
  float toIntervalPercentage = ((fromInterval + fromIntervalPercentage) / (fromMapSize - 1)) * (toMapSize - 1);
  byte toInterval = (byte)toIntervalPercentage;
  toIntervalPercentage = toIntervalPercentage - toInterval;

  //Linear interpolate
  return toMap[toInterval] + toIntervalPercentage * (toMap[toInterval + 1] - toMap[toInterval]);
}
int multiMbp(int value, int fromMap[], int fromMapSize, int toMap[], int toMapSize)
{
  //Boundary cases
  if (value <= fromMap[0]) return toMap[0];
  if (value >= fromMap[fromMapSize - 1]) return toMap[toMapSize - 1];

  //Find the fromMap interval that value lies in
  byte fromInterval = 0;
  while (value > fromMap[fromInterval + 1])
    fromInterval++;

  //Find the percentage of the interval that value lies in
  float fromIntervalPercentage = (float)(value - fromMap[fromInterval]) / (fromMap[fromInterval + 1] - fromMap[fromInterval]);

  //Map it to the toMap interval and percentage of that interval
  float toIntervalPercentage = ((fromInterval + fromIntervalPercentage) / (fromMapSize - 1)) * (toMapSize - 1);
  byte toInterval = (byte)toIntervalPercentage;
  toIntervalPercentage = toIntervalPercentage - toInterval;

  //Linear interpolate
  return toMap[toInterval] + toIntervalPercentage * (toMap[toInterval + 1] - toMap[toInterval]);
}
int multiMcp(int value, int fromMap[], int fromMapSize, int toMap[], int toMapSize)
{
  //Boundary cases
  if (value <= fromMap[0]) return toMap[0];
  if (value >= fromMap[fromMapSize - 1]) return toMap[toMapSize - 1];

  //Find the fromMap interval that value lies in
  byte fromInterval = 0;
  while (value > fromMap[fromInterval + 1])
    fromInterval++;

  //Find the percentage of the interval that value lies in
  float fromIntervalPercentage = (float)(value - fromMap[fromInterval]) / (fromMap[fromInterval + 1] - fromMap[fromInterval]);

  //Map it to the toMap interval and percentage of that interval
  float toIntervalPercentage = ((fromInterval + fromIntervalPercentage) / (fromMapSize - 1)) * (toMapSize - 1);
  byte toInterval = (byte)toIntervalPercentage;
  toIntervalPercentage = toIntervalPercentage - toInterval;

  //Linear interpolate
  return toMap[toInterval] + toIntervalPercentage * (toMap[toInterval + 1] - toMap[toInterval]);
}
