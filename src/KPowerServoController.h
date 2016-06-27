#ifndef KPowerServoController_h
#define KPowerServoController_h
#include <Arduino.h>
#include <Servo.h>

class KPowerServoController {
  private:
  unsigned long last_updated_at_millisec;

  int read_pin;
  int write_pin;
  int current_microsec;
  int target_degree;
  int target_microsec;
  int max_change_microsec_per_sec;
  int min_microsec;
  int max_microsec;
  Servo myservo;

  int sensor_value_to_degree(int sensor_value);
  int sensor_value_to_microsec(int sensor_value);
  int degree_to_microsec(int degree);
  int microsec_to_degree(int microsec);
  void update_current_microsec();

  public:
  int degree_buff;

  KPowerServoController(int servo_write_pin, int servo_read_pin);
  void set_min_degree(int degree);
  void set_min_microsec(int microsec);
  void set_max_degree(int degree);
  void set_max_microsec(int microsec);
  void power_on();
  void power_off();
  int get_current_degree();
  int get_current_microsec();
  int get_sensor_value();
  int get_sensor_degree();
  int get_sensor_microsec();
  boolean set_target_degree(int new_target_degree);
  boolean set_target_microsec(int new_target_microsec);
  boolean set_max_change_degree_per_sec(int new_max_change_degree_per_sec);
  boolean set_max_change_microsec_per_sec(int new_max_microsec_per_sec);
  void update_angle();
};

#endif
