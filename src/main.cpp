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
#include "config.h"

void autonomous() {

}

void drivercontrol() {
  //adds control function feedbacks
  robot::primary.ButtonL1.pressed(robot::liftUp);
  robot::primary.ButtonL2.pressed(robot::liftDown);
  robot::primary.ButtonR1.pressed(robot::backToggle);
  
  while (1) {
    //uses modifers from config unless turbo button is pressed, then uses maximum power
    const vec2 modifiers = robot::primary.ButtonR1.pressing() ? vec2{ 1.1, 1.1 } : controller_modifiers;
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis1.value(), modifiers.x, modifiers.y);
  }
}

int main() {
  vex::competition competition;

  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);

  while (1) vex::task::sleep(100);
}
