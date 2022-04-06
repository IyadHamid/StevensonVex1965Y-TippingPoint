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

//prints mode as given; string of mode
const void modePrint(const char* mode) {
  robot::primary.Screen.setCursor(0, 0);
  robot::primary.Screen.clearLine(0);
  robot::primary.Screen.print(mode);
}

// prints location and heading
const void locPrint() {
  robot::primary.Screen.setCursor(3, 0);
  robot::primary.Screen.clearLine(3);
  const vec2 loc = robot::idrive.getLocation();
  robot::primary.Screen.print("%.2f, %.2f, %.2f", loc.x, loc.y, robot::idrive.heading());
}

void autonomous() {

  using namespace robot;
#if defined(AUTON_A) //goal rush
  modePrint("Auton A");
  robot::idrive.reset();

  vex::thread clawThread([]{
    until(idrive.position() >= idrive.getDistanceRatio() * 44.0);
    frontClaw.set(true);
  });

  frontClaw.set(false);

  idrive.driveTo(48.0);
  
  //frontClaw.set(true);
  
  idrive.drive_percentage(-100);
  vex::this_thread::sleep_for(100);
  idrive.driveTo({0, 0}, true, true);
  clawThread.interrupt();
#elif defined(AUTON_B) //goal rush, leave room
  modePrint("Auton B");
  vex::thread clawThread([]{
    until(idrive.position() >= idrive.getDistanceRatio() * 44.0);
    frontClaw.set(true);
  });

  frontClaw.set(false);

  idrive.driveTo(48.0);
  
  idrive.drive_percentage(-100);
  idrive.driveTo(-5.0);
  idrive.turnTo(.25);
  clawThread.interrupt();

#elif defined(AUTON_C) //nothing
  modePrint("Auton C");
  robot::idrive.reset();

#elif defined(AUTON_D) //debug
  static bool a = true;
  if (a)
    idrive.driveTo({0.0, 0.0});
  else 
    idrive.driveTo({20.0, 0.0 });
  a = !a;
#endif
}

// toggles front claw and rumbles controller accordingly
void toggleclawcontrol() {
  //toggle front claw
  static bool isOpen = false; //cylinder starts out closed
  static vex::thread rumbleThread; //thread to rumble controller
  static auto rumbleFunc{[]{ //rumbles thread until interrupted
      while (1) { 
        robot::primary.rumble("-");
        vex::this_thread::sleep_for(500);
      }
  }};

  if (isOpen)
    rumbleThread.interrupt();
  else {
    rumbleThread = vex::thread(rumbleFunc);
    rumbleThread.setPriority(vex::thread::threadPrioritylow);
  }
  robot::frontClaw.set(isOpen = !isOpen);
}

// admin/testing controls
void admincontrol() {
  if (robot::primary.ButtonDown.pressing()) //resets location
    robot::idrive.reset();
  else if (robot::primary.ButtonLeft.pressing())  //runs auton
    autonomous();
}

// controls lift
void liftcontrol() {
  double pos = robot::lift.position(vex::rotationUnits::rev);
  if (robot::primary.ButtonL1.pressing() && pos < lift_up) //go up
    robot::lift.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  else if (robot::primary.ButtonL2.pressing() && pos > lift_down) //go down
    robot::lift.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  else 
    robot::lift.stop(vex::brakeType::hold);

  if (pos > lift_intake_thresh)
    robot::intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  else
    robot::intake.stop(vex::brakeType::hold);
}

void drivercontrol() {
  modePrint("Driver");
  
  robot::primary.ButtonR2.pressed(toggleclawcontrol);
  
  robot::primary.ButtonRight.pressed([]{ //toggle back claw
    static bool isOpen = false; //cylinder starts out closed
    robot::backClaw.set(isOpen = !isOpen);
  });
  robot::primary.ButtonY.pressed([]{ //toggle intake claw
    static bool isRunning = false; //cylinder starts out closed
    isRunning = !isRunning;
    if (isRunning)
      robot::intake.spin(directionType::fwd, 100, vex::percentUnits::pct);
    else 
      robot::intake.stop();
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
    if (robot::primary.ButtonUp.pressing())
      admincontrol();
#endif 
    liftcontrol();

  }
}

int main() {
  vex::competition competition;
  robot::frontClaw.open();
  robot::backClaw.open();
  robot::init();
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);
  
  vex::thread rainbowThread([]{
    while (1)  {
      static int hue = 0;
      //cycles through hues and clears screen with color
      robot::brain.Screen.clearScreen(++hue %= 360); 
      vex::this_thread::sleep_for(20); //sleeps to slow rainbow to a non-epileptic rate
    }
  });

  while (1)  {
    locPrint();
    vex::this_thread::sleep_for(500); //sleeps to minimize cpu usage and network usage
  }
}
