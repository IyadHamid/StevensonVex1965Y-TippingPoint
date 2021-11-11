/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.h                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed 15 Sep 2021                                           */
/*    Description:  Defines robot namespace                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "robot.h"

#include "config.h"

namespace robot {
  //initalizes brain
  vex::brain brain;
  //initalizes primary controller
  vex::controller primary;

  //initializes all 8 motors with data in config.h
  vex::motor lfront(std::get<0>(left_front ), std::get<1>(left_front ), std::get<2>(left_front ));
  vex::motor ltop  (std::get<0>(left_top   ), std::get<1>(left_top   ), std::get<2>(left_top   ));
  vex::motor lback (std::get<0>(left_back  ), std::get<1>(left_back  ), std::get<2>(left_back  ));
  vex::motor rfront(std::get<0>(right_front), std::get<1>(right_front), std::get<2>(right_front));
  vex::motor rtop  (std::get<0>(right_top  ), std::get<1>(right_top  ), std::get<2>(right_top  ));
  vex::motor rback (std::get<0>(right_back ), std::get<1>(right_back ), std::get<2>(right_back ));
  vex::motor lift  (std::get<0>(claw_lift  ), std::get<1>(claw_lift  ), std::get<2>(claw_lift  ));
  vex::motor back  (std::get<0>(back_lift  ), std::get<1>(back_lift  ), std::get<2>(claw_lift  ));

  //initalizes left motor group with left motors
  vex::motor_group lgroup (lfront, ltop, lback);
  //initalizes right motor group with right motors
  vex::motor_group rgroup(rfront, rtop, rback); 

  //initalizes inertial sensor with port from config.h
  vex::inertial isensor(inertial_port);

  //initalizes intelligent drivetrain with above sensor, motor groups, and constants from config.h
  inteldrive idrive(
    isensor, 
    lgroup, 
    rgroup,
    drive_k, turn_k,
    inches2units_ratio, robot_width
  );

  //initalizes claw with triport from config.h
  vex::pneumatics claw(brain.ThreeWirePort.CLAW_PORT);
}

void robot::liftUp() {

}

void robot::liftDown() {

}

void robot::backToggle() {

}
