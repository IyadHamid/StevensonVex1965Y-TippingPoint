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
#include "deltaTracker.h"

void autonomous() {
  using namespace robot;
  vex::this_thread::setPriority(vex::thread::threadPriorityHigh);
  robot::primary.Screen.setCursor(0, 0);
  robot::primary.Screen.clearLine();

#if defined(AUTON_A)
  //robot::hook.open();
  vex::thread t1([]{
    while (idrive.position() < idrive.getDistanceRatio() * 43.5)
      vex::this_thread::sleep_for(50);
    claw.open();
  });
  idrive.driveTo(45, 100);
  vex::this_thread::sleep_for(150);
  idrive.driveTo(-41, 100);
  t1.join();
  claw.close();

#elif defined(AUTON_B)

  idrive.driveTo(80);
  idrive.turnTo(.75);
  idrive.driveTo(48);
  idrive.turnTo(.75);
  idrive.driveTo(80);

  idrive.turnTo(.25);
  idrive.driveTo(48);

#else // defined(C)
  vex::thread t2([]{
    backSet(false);
    hook.open();
  });
  idrive.driveTo(-45);
  t2.join();
  back.rotateTo(back_high, vex::rotationUnits::rev, 110, vex::velocityUnits::pct, false);
#endif
}

void drivercontrol() {
  vex::this_thread::setPriority(vex::thread::threadPriorityHigh);
  robot::primary.Screen.setCursor(0, 0);
  robot::primary.Screen.clearLine();
  robot::primary.Screen.print("Driver");
  
  //adds control function feedbacks
  robot::primary.ButtonL1.pressed([]{ robot::liftSet(true); } );
  robot::primary.ButtonL2.pressed([]{ robot::liftSet(false); } );
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
    
    if (robot::primary.Axis1.value() == 0 && robot::primary.Axis2.value() == 0 && 
        robot::primary.Axis3.value() == 0 && robot::primary.Axis4.value() == 0) {
      robot::idrive.stop();
    }
    else {
#if defined(ARCADE)
      robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis4.value(), modifiers.x, modifiers.y);
#elif defined(DRONE)
      robot::idrive.arcade(robot::primary.Axis3.value(), robot::primary.Axis1.value(), modifiers.x, modifiers.y);
#else //defined(TANK)
      robot::idrive.tank(robot::primary.Axis3.value(), robot::primary.Axis2.value(), modifiers.x);
#endif
    }


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

  //robot::idrive.driveTo(vec2{20.0, 0.0});

  vex::this_thread::sleep_for(200);
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);

  vex::thread rainbowThread([]{
    while (1)  {
      static int hue = 0;
      robot::brain.Screen.clearScreen(++hue %= 360);
    vex::this_thread::sleep_for(20); //sleeps to slow rainbow
    }
  });

  while (1)  {
    auto loc = robot::idrive.getLocation();
    robot::primary.Screen.setCursor(5, 0);
    robot::primary.Screen.clearLine();
    robot::primary.Screen.print("%.2f, %.2f", loc.x, loc.y);
    vex::this_thread::sleep_for(100); //sleeps to minimize cpu usage
  }
}
