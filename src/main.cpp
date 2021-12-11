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
  vex::thread t([]{
    robot::backToggle();
  });
  robot::idrive.driveTo(20, 100);
  t.join();
}

void drivercontrol() {
  //adds control function feedbacks
  robot::primary.ButtonL1.pressed([]{ robot::liftAnalog(true); } );
  robot::primary.ButtonL2.pressed([]{ robot::liftAnalog(false); } );
  robot::primary.ButtonA.pressed(robot::backToggle);
  robot::primary.ButtonR2.pressed([]{ //toggle claw
    static bool isOpen = false; //piston starts out closed
    static vex::thread rumbleThread; //thread to rumble controller
    if (isOpen) {
      robot::claw.close();
      rumbleThread.interrupt();
    }
    else {
      robot::claw.open();
      rumbleThread = vex::thread([]{ //rumbles thread until interrupted
        while (1) {
          robot::primary.rumble("-");
          vex::this_thread::sleep_for(500);
        }
      });
      rumbleThread.setPriority(vex::thread::threadPrioritylow);
    }
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

    

    if (robot::primary.ButtonL1.pressing()) {
      if (robot::liftAnalog(true))
        robot::primary.rumble(".");
    }
    else if (robot::primary.ButtonL2.pressing()) {
      if (robot::liftAnalog(false))
        robot::primary.rumble(".");
    }
    else 
      robot::lift.stop(vex::brakeType::hold);


    if (robot::primary.ButtonUp.pressing()) { //enables manual control
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

  using namespace robot;
  double dist = 15;
  idrive.driveTo(dist, 100);
  idrive.turnTo(.25, 100);
  idrive.driveTo(dist, 100);
  idrive.turnTo(.25, 100);
  idrive.driveTo(dist, 100);
  idrive.turnTo(.25, 100);
  idrive.driveTo(dist, 100);
  idrive.turnTo(.25, 100);

  while (1) 
    vex::task::sleep(100); //sleeps to save cpu resources
}
