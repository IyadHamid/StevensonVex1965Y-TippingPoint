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

// type of auton, fallback is auton red
int autonType = 4;

// prints mode as given; string of mode
const void modePrint(const char* mode) {
  //robot::primary.Screen.setCursor(0, 0);
  //robot::primary.Screen.clearLine(0);
  //robot::primary.Screen.print(mode);
}

// prints location and heading
const void locPrint() {
  robot::primary.Screen.setCursor(3, 0);
  robot::primary.Screen.clearLine(3);
  const vec2 loc = robot::idrive.getLocation();
  robot::primary.Screen.print("%.2f, %.2f, %.2f", loc.x, loc.y, robot::idrive.heading());
}

// rush goal
void goalRush(double dist, double clawDist, double retreatDist) {
  using namespace robot;
  static double cd = clawDist;
  vex::thread clawThread([]{
    until(idrive.position() >= idrive.getDistanceRatio() * cd);
    frontClaw.set(false);
  });

  frontClaw.set(true);

  //idrive.driveTo(48.0);
  idrive.drive(110);
  until(idrive.position() >= idrive.getDistanceRatio() * dist);
  frontClaw.set(false);
  idrive.drive(-100);
  clawThread.interrupt();
  until(idrive.position() <= idrive.getDistanceRatio() * retreatDist);
}

// rush auton
void autonred() {
  using namespace robot;
  goalRush(40.0, 38.0, 20.0);
  idrive.driveTo({0.0, 0.0}, true);
  idrive.stop();
}

// rush and pick up alliance mogo auton
void autonorange() {
  using namespace robot;
  
  goalRush(40.0, 38.0, 20.0);

  //grab alliance mogo
  double goalXPos = 14.5;
  idrive.driveTo({goalXPos, -1.0}, true);
  lift.spinTo(.5, vex::rotationUnits::rev, false);
  backClaw.set(true);
  vex::this_thread::sleep_for(100);
  idrive.turnTo(-0.25, 0.0, false);
  idrive.drive(-35);
  vex::this_thread::sleep_for(1000); //time based as using wall to align
  backClaw.set(false);
  
  //pick up rings
  double ringYPos = 11.0;
  idrive.driveTo({goalXPos, ringYPos});
  intake.spin(vex::directionType::fwd);
  idrive.driveTo({60.0, ringYPos}, false, 30.0);
  idrive.driveTo({0.0, ringYPos}, true);

  intake.stop();
  backClaw.set(true);
}

// center rush auton
void autonyellow() {
  using namespace robot;
  //starts tilted
  idrive.inertialSensor.setHeading(326.3, vex::rotationUnits::deg);
  goalRush(49.0, 47.0, 30.0);
  
  double goalXPos = 24.0;
  lift.spinTo(.5, vex::rotationUnits::rev, false);
  backClaw.set(true);
  idrive.driveTo({goalXPos, 10.0}, true, 50.0);
  vex::this_thread::sleep_for(100);
  idrive.turnTo(-0.25, 50.0, false);
  idrive.drive(-35);
  vex::this_thread::sleep_for(1000); //time based as using wall to align
  backClaw.set(false);
  
  //pick up rings
  double ringYPos = 13.0;
  idrive.driveTo({goalXPos, ringYPos});
  intake.spin(vex::directionType::fwd);
  idrive.driveTo({60.0, ringYPos}, false, 30.0);
  idrive.driveTo({0.0, ringYPos}, true);

  intake.stop();
  backClaw.set(true);
}

// left side auton
void autongreen() {
  using namespace robot;
  goalRush(42.5, 40.5, 20.0);

  // grab alliance goal
  lift.spinTo(1.4, vex::rotationUnits::rev, false);
  idrive.driveTo({-6.5, -19.0}, true, 70.0);
  vex::this_thread::sleep_for(200);
  until(lift.position(vex::rotationUnits::rev) > 1.3); // guarentee we can swing mogo over wall
  idrive.turnTo(-0.25, 40.0, false);
  backClaw.set(true);
  vex::this_thread::sleep_for(500);
  idrive.driveTo(-28, 70.0);
  backClaw.set(false);
  vex::this_thread::sleep_for(500);
  idrive.driveTo(18.0);
  intake.spin(directionType::fwd);

  // intake placed rings
  for (int i = 0; i < 3; i++) {
    idrive.driveTo(16.0, 30.0);
    idrive.driveTo(-16.0);
    vex::this_thread::sleep_for(200);
  }
  backClaw.set(true);
  intake.stop();

}

void winPointAuton() {
  using namespace robot;
  lift.spinTo(.5, vex::rotationUnits::rev, false);
  //idrive.driveTo(2.0);
  until(lift.position(vex::rotationUnits::rev) > 0.45);
  frontClaw.set(true);
  vex::this_thread::sleep_for(100);
  idrive.driveTo({0.0, -22.0}, true);
  idrive.driveTo({75.0, -22.0}, true);
  backClaw.set(true);
  vex::this_thread::sleep_for(200);
  idrive.drive(-35);
  vex::this_thread::sleep_for(1500);
  idrive.stop();
  backClaw.set(false);
  vex::this_thread::sleep_for(200);
  intake.spin(vex::directionType::fwd);
  idrive.driveTo({75.0, -22.0});
  //idrive.turnTo(.625, 0.0, false);
  lift.spinTo(0.0, vex::rotationUnits::rev, false);
  intake.stop();
  vex::this_thread::sleep_for(100);
  //idrive.driveTo(40.0);
  idrive.driveTo({51.0, -51.0});
  frontClaw.set(false);
  idrive.driveTo({52.0, -22.0}, true);
}

// win point auton with center left mogo
void autonblue() {
  using namespace robot;
  winPointAuton();
}

// win point auton with center mid mogo
void autonpurple() {
  using namespace robot;
  winPointAuton();
}

void autonomous() {
  switch (autonType) {
    case 0:
      autonred();
      break;
    case 1:
      autonorange();
      break;
    case 2:
      autonyellow();
      break;
    case 3:
      autongreen();
      break;
    case 4:
      autonblue();
      break;
    case 5:
      autonpurple();
      break;
    default: //use auton red as a fallback
      autonred();
      break;
  }
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

  if (!isOpen)
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
  
  if (pos < lift_intake_thresh && robot::intakeRunning) { //stop intake running if too low
    robot::intakeRunning = false;
    robot::intake.stop();
  }
    
}

void drivercontrol() {
  modePrint("Driver");
  
  robot::primary.ButtonR2.pressed(toggleclawcontrol);
  
  robot::primary.ButtonRight.pressed([]{ //toggle back claw
    static bool isOpen = false; //cylinder starts out closed
    robot::backClaw.set(isOpen = !isOpen);
  });
  robot::primary.ButtonY.pressed([]{ //toggle intakes
    //prevent intakes from running into lift
    if (robot::lift.position(vex::rotationUnits::rev) < lift_intake_thresh)
      return;
    robot::intakeRunning = !robot::intakeRunning;
    if (robot::intakeRunning)
      robot::intake.spin(directionType::fwd, 100, vex::percentUnits::pct);
    else 
      robot::intake.stop();
  });
  //Runs intake backwards when B is held
  robot::primary.ButtonB.pressed([]{ 
    if (robot::lift.position(vex::rotationUnits::rev) > lift_intake_thresh) {
      robot::intakeRunning = true;
      robot::intake.spin(vex::directionType::rev); 
    }
  });
  robot::primary.ButtonB.released([]{ robot::intake.stop(); });


  while (1) {
    //if turbo button is pressed, use maximum power, else use controller modifers from config.h
    const vec2 modifiers = robot::primary.ButtonR1.pressing() ? vec2{ 1.1, 1.1 } : controller_modifiers;

    if (robot::primary.Axis1.value() == 0 && robot::primary.Axis2.value() == 0 && 
        robot::primary.Axis3.value() == 0 && robot::primary.Axis4.value() == 0) {
      robot::idrive.stop(vex::brakeType::hold);
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

  robot::init();
  competition.autonomous(autonomous);
  competition.drivercontrol(drivercontrol);
  
  vex::thread locPrintThread([]{
    while (1)  {
      locPrint();
      vex::this_thread::sleep_for(500); //sleeps to minimize cpu usage and network usage
    }
  });

  //renders tiles and sets correct auton
  std::array<vex::color, 6> autonColors{ vex::color::red  , vex::color::orange, vex::color::yellow, 
                                         vex::color::green, vex::color::blue  , vex::color::purple  };
  int screenWidth = 480, screenHeight = 272;
  int tileWidth = screenWidth / 3, tileHeight = screenHeight / 2;
  for (int i = 0; i < autonColors.size(); i++) {
    const auto color = autonColors[i];
    robot::brain.Screen.drawRectangle((i % 3) * tileWidth, (i / 3) * tileHeight, tileWidth, tileHeight, color);
  }
  until(robot::brain.Screen.pressing());
  int tileX = robot::brain.Screen.xPosition() / tileWidth;
  int tileY = robot::brain.Screen.yPosition() / tileHeight;
  autonType = tileX + tileY * 3;
  robot::brain.Screen.clearScreen(autonColors[autonType]);

  vex::this_thread::sleep_for(500);
  //callibrates inertial sensors again
  robot::idrive.inertialSensor.calibrate();
  //waits until it is done calibrating
  until(!robot::idrive.inertialSensor.isCalibrating());

  while (1)  {
    static int hue = 0;
    //cycles through hues and clears screen with color
    robot::brain.Screen.clearScreen(++hue %= 360); 
    vex::this_thread::sleep_for(20); //sleeps to slow rainbow to a non-epileptic rate
  }
}
