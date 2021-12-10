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
  robot::primary.ButtonL1.pressed([]{ robot::liftSet(true); } );
  robot::primary.ButtonL2.pressed([]{ robot::liftSet(false); } );
  robot::primary.ButtonA.pressed(robot::backToggle);
  robot::primary.ButtonR2.pressed([]{ //toggle claw
    static bool isOpen = false;
    if (isOpen) 
      robot::claw.close();
    else
      robot::claw.open(); 
    isOpen = !isOpen;
  });
  
  while (1) {
    //if turbo button is pressed, use maximum power, else use controller modifers from config.h
    const vec2 modifiers = robot::primary.ButtonR1.pressing() ? vec2{ 1.1, 1.1 } : controller_modifiers;
#if defined(ARCADE)
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis4.value(), modifiers.x, modifiers.y);
#elif defined(DRONE)
    robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis1.value(), modifiers.x, modifiers.y);
#else //defined(TANK)
    robot::idrive.tank(robot::primary.Axis3.value(), robot::primary.Axis2.value(), modifiers.x);
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
  //competition.drivercontrol(drivercontrol);
  
  //robot::idrive.driveTo(40, 100);
  robot::idrive.turnTo(.5, 100);
  
  //robot::idrive.turnTo(.5, 100);

  while (1) 
    vex::task::sleep(100); //sleeps to save cpu resources
}
