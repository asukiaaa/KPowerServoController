#include "KPowerServoController.h"

KPowerServoController::KPowerServoController(int servo_write_pin, int servo_read_pin) {
  write_pin = servo_write_pin;
  read_pin  = servo_read_pin;
  min_axis  = 0;
  max_axis  = 180;
  axis_buff = 3;
  max_change_axis_per_update = 1;
}

int KPowerServoController::sensor_value_to_axis(int sensor_value) {
  return (sensor_value - 130) / 4.2;
}

void KPowerServoController::update_current_axis() {
  if ( current_axis == target_axis ) { return; }

  if ( current_axis < target_axis ) {
    if ( target_axis - current_axis > max_change_axis_per_update ) {
      current_axis = current_axis + max_change_axis_per_update;
    } else {
      current_axis = target_axis;
    }
  } else {
    if ( current_axis - target_axis > max_change_axis_per_update) {
      current_axis = current_axis - max_change_axis_per_update;
    } else {
      current_axis = target_axis;
    }
  }
}

void KPowerServoController::write_servo_axis() {
  // ignore writing when target and current is in buff
  //if ( current_axis - target_axis < axis_buff && target_axis - current_axis < axis_buff ) {
  //  return;
  //}

  myservo.write(current_axis);
}

void KPowerServoController::power_on() {
  myservo.attach(write_pin);
  current_axis = get_sensor_axis();
  target_axis = get_sensor_axis();
}

void KPowerServoController::power_off() {
  myservo.detach();
}

int KPowerServoController::get_current_axis() {
  return current_axis;
}

int KPowerServoController::get_sensor_value() {
  return analogRead(read_pin);
}

int KPowerServoController::get_sensor_axis() {
  return sensor_value_to_axis( get_sensor_value() );
}

boolean KPowerServoController::set_target_axis(int new_target_axis) {
  if ( new_target_axis < min_axis ) {
    target_axis = min_axis;
    return false;

  } else if ( new_target_axis > max_axis ) {
    target_axis = max_axis;
    return false;
  }

  target_axis = new_target_axis;
  return true;
}

void KPowerServoController::update_axis() {
  update_current_axis();
  write_servo_axis();
}
