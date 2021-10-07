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
  vex::motor lmotor(vex::PORT2, vex::gearSetting::ratio18_1, false);
  vex::motor rmotor(vex::PORT10, vex::gearSetting::ratio18_1, true);
  vex::motor_group lft(lmotor);
  robot::brain.Screen.clearScreen(0);
  inteldrive tdrive(vex::inertial(0), 
                    vex::motor_group(lmotor), 
                    vex::motor_group(rmotor), 
                    1.0 / (2.0 * 1.5 * pi), 13.0); //ratio = 1.0 / (2.0 * 1.5 * pi)
  robot::brain.Screen.clearScreen();
  vex::task t([](void* td)->int{
    while (1) {
      robot::brain.Screen.printAt(0,120,"%f",((inteldrive*)td)->position());
      vex::task::sleep(100);
    }
    return 1;
  }, (void*)&tdrive);

  tdrive.driveTo(3, 80);
  while(1) vex::task::sleep(100);
  //while (true);
}
