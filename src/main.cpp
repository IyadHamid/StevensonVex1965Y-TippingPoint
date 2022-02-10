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

const void modePrint(const char* mode) {
  robot::primary.Screen.setCursor(0, 0);
  robot::primary.Screen.clearLine(0);
  robot::primary.Screen.print(mode);
}

void autonomous() {
  using namespace robot;

#if defined(AUTON_A)
  modePrint("Auton A");
  claw.close();
  deltaTracker<double> dist([]{ return idrive.position(); });
  vex::thread clawThread([]{
    until(idrive.position() >= idrive.getDistanceRatio() * 44.5);
    claw.open();
  });
  idrive.driveTo(45.5, false, false);
  backToggle();
  //claw.open();
  vex::this_thread::sleep_for(50);
  idrive.driveTo(18.0, true, false);
  idrive.turnTo(.75);
  idrive.driveTo(-15.0);
  backToggle();
  clawThread.join();
#elif defined(AUTON_B)
  modePrint("Auton B");
  claw.open();
  lift.rotateFor(1.8, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);
  idrive.driveTo(5);
  claw.close();
  idrive.driveTo(5);
  idrive.driveTo(-10);
#elif defined(AUTON_C)
  modePrint("Auton C");
  idrive.driveTo({0,0}, false, false);
#endif
}

void drivercontrol() {
  modePrint("Driver");
  
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

#ifdef ADMIN
    if (robot::primary.ButtonUp.pressing()) {
      if (robot::primary.ButtonDown.pressing()) { //resets location
        robot::idrive.reset();
      }
      else if (robot::primary.ButtonLeft.pressing()) { //toggles hook
        static bool state = true;
        robot::hook.set(!state);
        state = !state;
      }
      else if (robot::primary.ButtonRight.pressing()) { //runs auton
        autonomous();
      }
    }
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
  }
}

int main() {
  vex::competition competition;
  robot::claw.open();
  robot::init();
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
    robot::primary.Screen.setCursor(3, 0);
    robot::primary.Screen.clearLine(3);
    const vec2 loc = robot::idrive.getLocation();
    robot::primary.Screen.print("%.2f, %.2f, %.2f", loc.x, loc.y, robot::idrive.heading());
    vex::this_thread::sleep_for(500); //sleeps to minimize cpu usage and network usage
  }
}
