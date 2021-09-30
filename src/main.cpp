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

void autonomous() {

}

void drivercontrol() {

}

int main() {
  vex::competition competition;
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);
  vex::motor lmotor(2, vex::gearSetting::ratio18_1, false);
  vex::motor rmotor(10, vex::gearSetting::ratio18_1, false);
  robot::brain.Screen.clearScreen(0);
  inteldrive tdrive(vex::inertial(0), 
                    vex::motor_group(lmotor), 
                    vex::motor_group(rmotor), 
                    0.05658842421, 13.0); //ratio = 1.0 / ((60.0/32.0) * 2.0 * 1.5 * pi)
  robot::brain.Screen.clearScreen();

  tdrive.driveTo(10, 80);

  while (true);
}
