#include <SimpleFOC.h>
#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8


// BLDC motor & driver instance
// BLDCMotor motor = BLDCMotor(pole pair number);
BLDCMotor motor = BLDCMotor(7, 1, 360);
// BLDCDriver3PWM driver = BLDCDriver3PWM(pwmA, pwmB, pwmC, Enable(optional));
BLDCDriver3PWM driver = BLDCDriver3PWM(IN1, IN2, IN3);