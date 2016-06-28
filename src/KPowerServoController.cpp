#include "KPowerServoController.h"

KPowerServoController::KPowerServoController(int servo_write_pin, int servo_read_pin) {
  write_pin = servo_write_pin;
  read_pin  = servo_read_pin;
  set_min_degree(0);
  set_max_degree(180);
  degree_buff = 3;
  set_max_change_degree_per_sec(30);
}

int KPowerServoController::sensor_value_to_degree(int sensor_value) {
  return (sensor_value - 130) / 4.2;
}

int KPowerServoController::sensor_value_to_microsec(int sensor_value) {
  // 138 -> 500
  // 528 -> 1500
  // 912 -> 2500
  //return (( sensor_value - 130 ) * 2000 / 780) + 500;
  return sensor_value * 2.56 + 180;
}

int KPowerServoController::degree_to_microsec(int degree) {
  // 0   -> 500
  // 90  -> 1500
  // 180 -> 2500
  return ( degree * 2000 / 180 ) + 500;
}

int KPowerServoController::microsec_to_degree(int microsec) {
  //  500 ->   0
  // 1500 ->  90
  // 2500 -> 180
  return ( microsec - 500 ) * 180 / 2000;
}

void KPowerServoController::update_current_microsec() {
  unsigned long current_millisec = millis();
  float max_change_microsec =
    max_change_microsec_per_sec * (current_millisec - last_updated_at_millisec) / 1000;

  //Serial.print("max_change: ");
  //Serial.println(max_change_microsec);
  //Serial.print("max_change_per_sec: ");
  //Serial.println(max_change_microsec_per_sec);
  //Serial.print("Current millisec: ");
  //Serial.println(current_millisec);
  //Serial.print("last millisec: ");
  //Serial.println(last_updated_at_millisec);

  last_updated_at_millisec = current_millisec;

  if ( current_microsec == target_microsec ) { return; }

  // update current_microsec with using max_change_microsec
  if ( current_microsec < target_microsec ) {
    if ( target_microsec - current_microsec > max_change_microsec ) {
      current_microsec = current_microsec + max_change_microsec;
    } else {
      current_microsec = target_microsec;
    }
  } else {
    if ( current_microsec - target_microsec > max_change_microsec) {
      current_microsec = current_microsec - max_change_microsec;
    } else {
      current_microsec = target_microsec;
    }
  }
}

void KPowerServoController::set_min_microsec(int microsec) {
  min_microsec = microsec;
}

void KPowerServoController::set_min_degree(int degree) {
  min_microsec = degree_to_microsec(degree);
}

void KPowerServoController::set_max_microsec(int microsec) {
  max_microsec = microsec;
}

void KPowerServoController::set_max_degree(int degree) {
  max_microsec = degree_to_microsec(degree);
}

void KPowerServoController::power_on() {
  myservo.attach(write_pin);

  current_microsec = get_sensor_microsec();
  target_microsec = current_microsec;

  myservo.writeMicroseconds(current_microsec);
}

void KPowerServoController::power_off() {
  if ( myservo.attached() ) {
    myservo.detach();
    //Serial.println("detached");
  }
}

int KPowerServoController::get_current_degree() {
  return microsec_to_degree(current_microsec);
}

int KPowerServoController::get_current_microsec() {
  return current_microsec;
}

int KPowerServoController::get_sensor_value() {
  return analogRead(read_pin);

  // return average of the sensor value
  //int value_sum = 0;
  //int value_check_loop_count = 5;
  //for ( int i=0; i<value_check_loop_count; i++ ) {
  //  value_sum += analogRead(read_pin);
  //}
  //return value_sum / value_check_loop_count;
}

int KPowerServoController::get_sensor_degree() {
  return sensor_value_to_degree( get_sensor_value() );
}

int KPowerServoController::get_sensor_microsec() {
  return sensor_value_to_microsec( get_sensor_value() );
}

int KPowerServoController::get_target_microsec() {
  return target_microsec;
}

boolean KPowerServoController::set_target_degree(int new_target_degree) {
  set_target_microsec( degree_to_microsec(new_target_degree) );
}

boolean KPowerServoController::set_target_microsec(int new_target_microsec) {
  if ( new_target_microsec < min_microsec ) {
    target_microsec = min_microsec;
    return false;

  } else if ( new_target_microsec > max_microsec ) {
    target_microsec = max_microsec;
    return false;
  }

  target_microsec = new_target_microsec;
  //Serial.println(target_microsec);
  return true;
}

boolean KPowerServoController::set_max_change_degree_per_sec(int new_max_change_degree_per_sec) {
  set_max_change_microsec_per_sec( degree_to_microsec(new_max_change_degree_per_sec) -
                                   degree_to_microsec(0) );
}

boolean KPowerServoController::set_max_change_microsec_per_sec(int new_max_change_microsec_per_sec) {
  max_change_microsec_per_sec = new_max_change_microsec_per_sec;
}

void KPowerServoController::update_angle() {
  update_current_microsec();
  myservo.writeMicroseconds(current_microsec);
}
