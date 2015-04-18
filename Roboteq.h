#ifndef ROBOTEQ_H
#define ROBOTEQ_H

//#include "Arduino.h"

#define ROBOTEQ_SPEED 12        // Pins
#define ROBOTEQ_DIR 13
#define ROBOTEQ_BRAKE 29

#define LINEAR_TO_SPEED 4000
#define ANGULAR_TO_DIR 4000   //???
#define MAX 		1000
#define MIN			-1000
#define BAUD 		115200

class roboteq
{
  public:
  roboteq();	
  //int roboteq_init();
  int roboteq_set_speed(int speed);
  int roboteq_set_direction(int direction);
  int roboteq_get_speed();
  int roboteq_get_direction();
  void roboteq_close();
  
  private:
  int curr_speed;
  int curr_direction;	
  String dir_str;
  String speed_str;
  String cmd_speed;
  String cmd_dir;
} ;

#endif
