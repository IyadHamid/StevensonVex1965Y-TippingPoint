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
  vex::brain brain;
  vex::controller primary;

  vex::motor lfront(std::get<0>(left_front  ), std::get<1>(left_front  ), std::get<2>(left_front  ));
  vex::motor ltop  (std::get<0>(left_top    ), std::get<1>(left_top    ), std::get<2>(left_top    ));
  vex::motor lback (std::get<0>(left_back   ), std::get<1>(left_back   ), std::get<2>(left_back   ));
  vex::motor rfront(std::get<0>(right_front ), std::get<1>(right_front ), std::get<2>(right_front ));
  vex::motor rtop  (std::get<0>(right_top   ), std::get<1>(right_top   ), std::get<2>(right_top   ));
  vex::motor rback (std::get<0>(right_back  ), std::get<1>(right_back  ), std::get<2>(right_back  ));

  vex::motor_group lgroup (lfront, ltop, lback);
  vex::motor_group rgroup(rfront, rtop, rback); 

  vex::inertial isensor(inertial_port);

  inteldrive idrive(
    isensor, 
    lgroup, 
    rgroup,
    drive_k, turn_k,
    inches2units_ratio, robot_width
  );

  vex::pneumatics claw(brain.ThreeWirePort.CLAW_PORT);

  
  void liftUp() {

  }

  void liftDown() {

  }

  void backToggle() {

  }
}