#include <SimpleFOC.h>

// motor instance
BLDCMotor motor = BLDCMotor(7);
// driver instance
//BLDCDriver3PWM driver = BLDCDriver3PWM(5, 18, 19, 23);
BLDCDriver3PWM driver = BLDCDriver3PWM(32, 33, 25, 22);

// Magnetic sensor instance
MagneticSensorI2C sensor0 = MagneticSensorI2C(AS5600_I2C);
MagneticSensorI2C sensor1 = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(0);

//Command settings
float target_angle = -3 * PI; // 180 degrees in radians
uint32_t prev_millis;

Commander command = Commander(Serial);
void doTarget(char *cmd)
{
  command.scalar(&target_angle, cmd);
}

void setup() {
   // use monitoring with serial 
  Serial.begin(115200);
  while(!Serial);
  SimpleFOCDebug::enable(&Serial);

  
  driver.voltage_power_supply = 12;

  I2Cone.begin(19,18, 400000UL);
  // initialize magnetic sensor hardware
  sensor0.init(&I2Cone);
  // link the motor to the sensor
  motor.linkSensor(&sensor0);

  // driver config
  driver.init();
  motor.linkDriver(&driver);

  // set motion control loop to be used
  motor.controller = MotionControlType::angle;

  // controller configuration 
  // default parameters in defaults.h

  // controller configuration based on the control type 
  // velocity PID controller parameters
  // default P=0.5 I = 10 D =0
  motor.PID_velocity.P = 0.042412;
  motor.PID_velocity.I = 0.00125;
  //motor.PID_velocity.D = 0;
  // jerk control using voltage voltage ramp
  // default value is 300 volts per sec  ~ 0.3V per millisecond
  //motor.PID_velocity.output_ramp = 1000;

  // velocity low pass filtering
  // default 5ms - try different values to see what is the best. 
  // the lower the less filtered
  motor.LPF_velocity.Tf = 0.01;

  motor.P_angle.P = 20;

  // since the phase resistance is provided we set the current limit not voltage
  // default 0.2
  motor.voltage_limit = 1.5;   // [V]
  motor.velocity_limit = 60; // [rad/s]
  motor.current_limit = 1; // [A]

  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();

  Serial.println("Motor ready.");
  command.add('T', doTarget, "target angle");
  _delay(1000);
}

// velocity set point variable
float target_velocity = 10; // 2Rad/s ~ 20rpm

// timestamp for changing direction
long timestamp_us = _micros();

void loop() {
  if(_micros() - timestamp_us > 1e6) {
      timestamp_us = _micros();
  //    // inverse angle
      target_angle = -target_angle;   
      Serial.print("Target angle: ");
      Serial.println(target_angle);
  }

  // main FOC algorithm function
  motor.loopFOC();

  // Motion control function
  motor.move(target_angle);

  command.run();
}