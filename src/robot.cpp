/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.h                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed 15 Sep 2021                                           */
/*    Description:  Defines robot namespace                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "robot.h"

constexpr int32_t inertial_port     = 1;

constexpr int32_t left_front_port   = 1;
constexpr int32_t left_middle_port  = 1;
constexpr int32_t left_back_port    = 1;

constexpr int32_t right_front_port  = 1;
constexpr int32_t right_middle_port = 1;
constexpr int32_t right_back_port   = 1;

constexpr double inches2units_ratio = 1.0;
constexpr double robot_width = 13.0;

// { proportional constant, integral constant, derivative constant, tolerance }
constexpr PID::kPID drive_k = { 1.0, 1.0, 1.0, 1.0 };
constexpr PID::kPID turn_k = { 1.0, 1.0, 1.0, 1.0 };

namespace robot {
  vex::brain brain;
  vex::controller primary;


  vex::motor lfront (left_front_port  , vex::gearSetting::ratio6_1, false);
  vex::motor lmiddle(left_middle_port , vex::gearSetting::ratio6_1, false);
  vex::motor lback  (left_back_port   , vex::gearSetting::ratio6_1, false);
  vex::motor rfront (right_front_port , vex::gearSetting::ratio6_1, false);
  vex::motor rmiddle(right_middle_port, vex::gearSetting::ratio6_1, false);
  vex::motor rback  (right_back_port  , vex::gearSetting::ratio6_1, false);

  inteldrive idrive(vex::inertial(inertial_port), 
                    vex::motor_group(lfront, lmiddle, lback), 
                    vex::motor_group(rfront, rmiddle, rback),
                    drive_k, turn_k,
                    inches2units_ratio, robot_width,
                    vex::velocityUnits::rpm);
}