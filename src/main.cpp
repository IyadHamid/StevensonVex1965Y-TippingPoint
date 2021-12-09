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
  robot::hook.open();
  vex::thread([]{
    robot::backToggle();
  });
}

void drivercontrol() {
  //adds control function feedbacks
  robot::primary.ButtonL1.pressed(robot::liftUp);
  robot::primary.ButtonL2.pressed(robot::liftDown);
  robot::primary.ButtonR1.pressed(robot::backToggle);
  
  while (1) {
    //if turbo button is pressed, use maximum power, else use controller modifers from config.h
    const vec2 modifiers = robot::primary.ButtonR1.pressing() ? vec2{ 1.1, 1.1 } : controller_modifiers;
#if DRIVE_MODE == ARCADE
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis4.value(), modifiers.x, modifiers.y);
#elif DRIVE_MODE == DRONE
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis1.value(), modifiers.x, modifiers.y);
#else //DRIVE_MODE == TANK
    robot::idrive.TANK(robot::primary.Axis3.value(), robot::primary.Axis2.value(), modifiers.x);
#endif

    //enables manual control
    if (robot::primary.ButtonUp.pressing()) {
      //prints options
      robot::brain.Screen.printAt(0, 20, "Left : toggle hooks");
      
      //toggles hook
      if (robot::primary.ButtonLeft.pressing()) {
        static bool state = true;
        robot::hook.set(!state);
        state = !state;
      }
    }
  }
}

int main() {
  vex::competition competition;
  
  robot::init();
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);
  while (1) 
    vex::task::sleep(100); //sleeps to save cpu resources
}
