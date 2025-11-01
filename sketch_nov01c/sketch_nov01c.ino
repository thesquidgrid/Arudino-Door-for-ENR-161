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

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

void setup() {
  Serial.begin(9600);
  
  pinMode(OUT_RED_LED, OUTPUT);
  pinMode(OUT_GREEN_LED, OUTPUT);
  pinMode(IN_VOLTAGE_LVL, INPUT);
  pinMode(QTR_LEFT, INPUT);
  pinMode(QTR_RIGHT, INPUT);
  pinMode(EN, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);  // Enable stepper motor driver
  stepper.setMaxSpeed(500);      // Steps per second
  stepper.setAcceleration(300);  // Acceleration for smoother movement
  
}

void loop() {
  
  checkBattery();
  delay(1000);

}

void checkBattery(){
  bool batteryLevel = digitalRead(IN_VOLTAGE_LVL);
  if(batteryLevel == HIGH){
    digitalWrite(OUT_GREEN_LED, HIGH);
    digitalWrite(OUT_RED_LED, LOW);
  }else{
    digitalWrite(OUT_GREEN_LED, LOW);
    digitalWrite(OUT_RED_LED, HIGH);
    // function for low power mode.
  }
}


String checkGateClose(){
  bool qtr_Left = digitalRead(QTR_LEFT);
  bool qtr_Right = digitalRead(QTR_RIGHT);
  String gateStatus;
  // both are covered and therefore are good. 
  if(qtr_Left == HIGH && qtr_Right == HIGH){
    gateStatus == "closed";
  } else {
    gateStatus == "opened";
  }
  return gateStatus;
}






// Function to Rotate 90 Degrees


void rotate90() {  

    stepper.move(TURN_90);  // Move exactly 50 steps
    stepper.runToPosition(); // Ensures it reaches the exact position

}
