#ifndef ROBOTEQ_H
#define ROBOTEQ_H

#include "Arduino.h"

#define ROBOTEQ_SPEED   12      // Pins
#define ROBOTEQ_DIR     13
#define ROBOTEQ_BRAKE   29

#define LINEAR_TO_SPEED 4000
#define ANGULAR_TO_DIR  4000    //???
#define MAX             1000
#define MIN             -1000
#define BAUD            115200

class roboteq
{
  public:
  roboteq();  
  int roboteq_init();
  String roboteq_set_speed_right(char c_speed);
  String roboteq_set_speed_left(char c_speed);
  int roboteq_set_direction(int direction);
  int roboteq_get_speed_right();
  int roboteq_get_speed_left();
  int roboteq_get_direction();
  void roboteq_close();
  
  private:
  int speed_l;
  int speed_r;
  int dir; 
  String dir_str;
  String speed_str_l;
  String speed_str_r;
} ;

#endif
