/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed Sep 15 2021                                           */
/*    Description:  V5 project starting point                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "common.h"
#include "robot.h"
#include <iostream>
void autonomous() {

}

void drivercontrol() {
  robot::idrive.arcade(robot::primary.Axis4.value(), robot::primary.Axis3.value());
}

int main() {
  vex::competition competition;
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);


  while(1) vex::task::sleep(100);
}
