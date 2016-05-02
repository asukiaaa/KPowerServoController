#ifndef KPowerServoController_h
#define KPowerServoController_h
#include <Arduino.h>
#include <Servo.h>

class KPowerServoController {
  private:
  int read_pin;
  int write_pin;
  int current_axis;
  int target_axis;
  Servo myservo;

  int sensor_value_to_axis(int sensor_value);
  void update_current_axis();
  void write_servo_axis();

  public:
  int min_axis;
  int max_axis;
  int axis_buff;
  int max_change_axis_per_update;

  KPowerServoController(int servo_write_pin, int servo_read_pin);
  void power_on();
  void power_off();
  int get_current_axis();
  int get_sensor_value();
  int get_sensor_axis();
  boolean set_target_axis(int new_target_axis);
  void update_axis();
};

#endif
