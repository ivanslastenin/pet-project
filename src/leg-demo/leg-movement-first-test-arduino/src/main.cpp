#include <SimpleFOC.h>
#include "./motor_init.h"


void setup() {
  Serial.begin(115200);
  while(!Serial);
  // deactivate the OUT4 output
  pinMode(IN4,OUTPUT);
  digitalWrite(IN4,LOW);

 // TCCR2B = 0b00000001;  // x1
  //TCCR2A = 0b00000001;  // phase correct

    // comment out if not needed
  SimpleFOCDebug::enable(&Serial);
  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 12;
  //driver.voltage_limit = 3;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor movements
  motor.voltage_limit = 4;   // [V]
  motor.velocity_limit = 20; // [rad/s]
  motor.current_limit = 2; // [A]

  // open loop control config
  motor.controller = MotionControlType::velocity_openloop;
  //motor.current_limit = 0.5; // [A]
 // motor.current_limit = 0.5; // [A]

  // init motor hardware
  motor.init();

  Serial.println("Motor ready!");
  _delay(1000);
}

float target_velocity = 1; // [rad/s]

void loop() {
  
  //unsigned long now_us = _micros();
  // open loop velocity movement
  // using motor.voltage_limit and motor.velocity_limit
  motor.move(target_velocity);

  
  //unsigned long now_us_1 = _micros();

  //Serial.print("Time us FOC loop: ");
  //Serial.println(now_us_1 - now_us);

}