#include <Arduino.h>
#include "QTRSensors.h"
#include <AccelStepper.h>

#define OUT_RED_LED  0
#define OUT_GREEN_LED 1
#define IN_VOLTAGE_LVL 2
#define QTR_LEFT 3
#define QTR_RIGHT 4
#define EN 5
#define STEP 6
#define DIR 7


#define TURN_90 50

QTRSensors qtr_L;
QTRSensors qtr_R;

const uint8_t SensorCount = 1;
uint16_t sensorValues[SensorCount];

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

void setup() {
  Serial.begin(9600);
  qtr_L.setTypeRC();
  qtr_R.setTypeRC();
  qtr_L.setSensorPins((const uint8_t[]) {
    QTR_LEFT
  }, 1);
  qtr_R.setSensorPins((const uint8_t[]) {
    QTR_RIGHT
  }, 1);
 
  
  pinMode(OUT_RED_LED, OUTPUT);
  pinMode(OUT_GREEN_LED, OUTPUT);
  pinMode(IN_VOLTAGE_LVL, INPUT);
  // do i need these?
  pinMode(QTR_LEFT, INPUT);
  pinMode(QTR_RIGHT, INPUT);
  //
  pinMode(EN, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  digitalWrite(EN, LOW);  // Enable stepper motor driver
  stepper.setMaxSpeed(500);      // Steps per second
  stepper.setAcceleration(300);  // Acceleration for smoother movement

  //DO NOT WORK IF BATTERY IS LOW.
//  if(checkBattery() == LOW){
//    lowPwrMode();
//    loop();
//  }
// putGateInStartingPos();
  Serial.println("------------------------------");
    int sel = Serial.parseInt();
    debugMenu(sel);
    debugPrintout();
}

void loop() {
  if (Serial.available() > 0) {
    Serial.println("------------------------------");
    int sel = Serial.parseInt();
    debugMenu(sel);
    debugPrintout();
  } else {
    int meow = Serial.read();
    delay(200);
  }
}

bool checkBattery(){
  bool batteryLevel = digitalRead(IN_VOLTAGE_LVL);
  if(batteryLevel == HIGH){
    digitalWrite(OUT_GREEN_LED, HIGH);
    digitalWrite(OUT_RED_LED, LOW);
  }else{
    digitalWrite(OUT_GREEN_LED, LOW);
    digitalWrite(OUT_RED_LED, HIGH);
    // function for low power mode.
  }

  return batteryLevel;
}


bool checkGateClose(){
  bool qtr_Left = digitalRead(QTR_LEFT);
  bool qtr_Right = digitalRead(QTR_RIGHT);
  bool gateStatus;
  // both are covered and therefore are good. 
  if(qtr_Left == HIGH && qtr_Right == HIGH){
    gateStatus = true;
  } else {
    gateStatus = false;
  }
  return gateStatus;
}


// Function to Rotate 90 Degrees

void rotate90() {  
    stepper.move(TURN_90);  // Move exactly 50 steps
    stepper.runToPosition(); // Ensures it reaches the exact position
}

void rotateStep() {  
    stepper.move(1);  // Move exactly 1 step
    stepper.runToPosition(); // Ensures it reaches the exact position
}

void lowPwrMode(){
  digitalWrite(EN, HIGH);
  digitalWrite(OUT_GREEN_LED, LOW);
  digitalWrite(OUT_RED_LED, HIGH);
}

void putGateInStartingPos(){
  bool gateClosed = checkGateClose();
  if(!gateClosed){
    digitalWrite(DIR, LOW);
    while(!gateClosed){
      bool gateClosed = checkGateClose();
      rotateStep();
    }
  }
}

void debugPrintout(){
  Serial.println("1. Turn stepper motor to the Left");
  Serial.println("2. Turn Stepper motor to the Right");
  Serial.println("3. Check value of QTR_L");
  Serial.println("4. check value of QTR_R");
  Serial.println("5. check value of battery");
  Serial.println("6. Put gate in starting position");
  Serial.println("7. Check battery level");
  Serial.println("input choice: ");
}

void debugMenu(int selection) {
  delay(300);
  switch (selection) {
    case 1: {
      digitalWrite(DIR, LOW);
      Serial.println("moving to the left");
      rotateStep();
    }
    break;
    case 2: {
      digitalWrite(DIR, HIGH);
      Serial.println("moving to the right");
      rotateStep();
    }
    break;
    case 3: {
      qtr_L.read(sensorValues);
      for (uint8_t i = 0; i < SensorCount; i++)
        {
          Serial.print("Value in qtr_Left is: ");
          Serial.print(sensorValues[i]);
          Serial.println();
        }
      }
    break;
    case 4: {
      qtr_R.read(sensorValues);
      for (uint8_t i = 0; i < SensorCount; i++)
        {
          Serial.print("Value in qtr_Left is: ");
          Serial.print(sensorValues[i]);
          Serial.println();
        }
      }
    break;
    case 5: {
      bool batVal = checkBattery();
      Serial.print("Battery is: ");
      Serial.println(batVal);
    }
    break;
    case 6: {
      delay(300);
      Serial.println("Gate Moving");
      putGateInStartingPos();
    }
    break;
    case 7: {
      
    }
    break;
  }
  
}
