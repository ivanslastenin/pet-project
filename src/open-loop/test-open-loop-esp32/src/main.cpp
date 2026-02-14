/// MKS ESP32 FOC Open loop speed control example; Test Library：SimpleFOC 2.1.1 ; Tested hardware：MKS ESP32 FOC V1.0
/// Enter "T+number" in the serial port to set the speed of the two motors.For example, to set the motor to rotate at a speed of 10rad/s, input "T10"
/// When the motor is powered on, it will rotate at 5rad/s by default
/// When using your own motor, do remember to modify the default number of pole pairs, the value in BLDCMotor(7).
/// The default power supply voltage of the program is 12V.
/// Please remember to modify the voltage_power_supply , voltage_limit variable values when using other voltages for power supply

#include <SimpleFOC.h>

BLDCMotor motor = BLDCMotor(7);                           //According to the selected motor, modify the number of pole pairs here, the value in BLDCMotor()
BLDCDriver3PWM driver = BLDCDriver3PWM(18, 19, 21, 22);
  

/// Target Variable
float target_velocity = 15;

void setup() {
  
  Serial.begin(115200);
  while(!Serial);

  

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

    // choose FOC modulation (optional)
  // limiting motor movements
  motor.voltage_limit = 1;   // [V]
  motor.velocity_limit = 30; // [rad/s]
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

void loop() {
  
  //unsigned long now_us = _micros();
  // open loop velocity movement
  // using motor.voltage_limit and motor.velocity_limit
  motor.move(target_velocity);

  
  //unsigned long now_us_1 = _micros();

  //Serial.print("Time us FOC loop: ");
  //Serial.println(now_us_1 - now_us);
}