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
  
}

int main() {
  vex::competition competition;
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);

  //TEST CODE
  vex::motor lmotor(vex::PORT2, vex::gearSetting::ratio18_1, false);
  vex::motor rmotor(vex::PORT10, vex::gearSetting::ratio18_1, true);
  vex::motor_group lft(lmotor);
  robot::brain.Screen.clearScreen(0);
  inteldrive tdrive(vex::inertial(vex::PORT18), 
                    vex::motor_group(lmotor), 
                    vex::motor_group(rmotor), 
                    {1.0, 0.01, 1.0, 0.1}, {1.0, 0.5, 1.0 , 0.05},
                    rev2rad(1.0 / (1.5 * tau)), 13.0, vex::velocityUnits::pct); //ratio = 1.0 / (1.5 * tau)
  robot::brain.Screen.clearScreen();

  tdrive.driveTo(5, 100);
  //tdrive.turnTo(deg2rad(-135), 10);
  //END TEST CODE

  while(1) vex::task::sleep(100);
}
