/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed Sep 15 2021                                           */
/*    Description:  V5 project starting point                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <iostream>

#include "common.h"
#include "robot.h"

void autonomous() {

}

void drivercontrol() {

  //inteldrive idrive(vex::inertial(inertial_port), 
  //                  vex::motor_group(robot::lfront, robot::lmiddle, robot::lback), 
  //                  vex::motor_group(robot::rfront, robot::rmiddle, robot::rback),
  //                  drive_k, turn_k,
  //                  inches2units_ratio, robot_width,
  //                  vex::velocityUnits::pct);
  while (1) {
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis1.value(), 1.1, 1.1);
  }
}

int main() {
  vex::competition competition;

  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);

  while(1) vex::task::sleep(100);
}
