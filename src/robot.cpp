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

  vex::motor lfront (std::get<0>(left_front  ), std::get<1>(left_front  ), std::get<2>(left_front  ));
  vex::motor lmiddle(std::get<0>(left_middle ), std::get<1>(left_middle ), std::get<2>(left_middle ));
  vex::motor lback  (std::get<0>(left_back   ), std::get<1>(left_back   ), std::get<2>(left_back   ));
  vex::motor rfront (std::get<0>(right_front ), std::get<1>(right_front ), std::get<2>(right_front ));
  vex::motor rmiddle(std::get<0>(right_middle), std::get<1>(right_middle), std::get<2>(right_middle));
  vex::motor rback  (std::get<0>(right_back  ), std::get<1>(right_back  ), std::get<2>(right_back  ));

  inteldrive idrive(vex::inertial(inertial_port), 
                    vex::motor_group(lfront, lmiddle, lback), 
                    vex::motor_group(rfront, rmiddle, rback),
                    drive_k, turn_k,
                    inches2units_ratio, robot_width,
                    vex::velocityUnits::rpm);
}