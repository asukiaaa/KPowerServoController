#include "KPowerServoController.h"
#include <Servo.h>

// Downloaded from https://github.com/fabriceo/SCoop
#include <SchedulerARMAVR.h>

KPowerServoController kp_servo(2, 0);

void setup() {
  Serial.begin(9600);
  kp_servo.power_on();
  Scheduler.startLoop(loop2);
  Scheduler.startLoop(loop3);
}

void loop() {
  delay(4000);
  kp_servo.set_target_degree(90);
  delay(4000);
  kp_servo.set_target_degree(180);
}

void loop2() {
  kp_servo.update_angle();
  delay(10);
}

void loop3() {
  Serial.println(kp_servo.get_sensor_value());
  Serial.println(kp_servo.get_sensor_degree());
  Serial.println(kp_servo.get_sensor_microsec());
  delay(500);
}

