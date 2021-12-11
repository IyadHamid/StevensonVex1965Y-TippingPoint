/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       inteldrive.cpp                                            */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Thu Sep 16 2020                                           */
/*    Description:  Definition of inteldrive                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "inteldrive.h"

#include <cassert>

#include "common.h"
#include "PID.h"

//#if DEBUG == LPS
#include "robot.h"
//#endif

inteldrive::inteldrive(vex::inertial i, 
                       vex::motor_group l, vex::motor_group r,
                       PID::kPID drive_k, PID::kPID turn_k,
                       double ratio, double rw)
: inertialSensor{i}, left{l}, right{r},
  drivePID{ //initalizes drivePID
    [=](double goal) { return goal - robot::idrive.position(); },
    [=](double input) { robot::idrive.drive(input); }, 
    drive_k
  },
  turnPID{ //initalizes turnPID
    [=](double goal) { return angle_difference_rev(goal, robot::idrive.heading()); }, //turnPID internally uses radians
    [=](double input) {
      robot::idrive.left .spin(vex::directionType::fwd, input, vex::velocityUnits::pct);
      robot::idrive.right.spin(vex::directionType::rev, input, vex::velocityUnits::pct);
    },
    turn_k
  },
  robotWidth{rw}, distanceRatio{ratio}
{
  //callibrates inertial sensors
  inertialSensor.calibrate();
  //waits until it is done calibrating
  while (inertialSensor.isCalibrating())
    vex::this_thread::sleep_for(100); //sleeps to save cpu resources
  //lpsThread = MEMBER_FUNCTION_THREAD( inteldrive, runLPS() );
}

inteldrive::inteldrive() //initalizes an unusable/empty inteldrive
: inertialSensor(-1),
  drivePID{ nullptr, nullptr, { 0.0, 0.0, 0.0, 0.0 } },
  turnPID { nullptr, nullptr, { 0.0, 0.0, 0.0, 0.0 } }
{}

double inteldrive::heading(vex::rotationUnits units) {
  //redirects to inertial sensor
  return inertialSensor.heading(units);
}

double inteldrive::heading() {
  //redirects to heading(units) with revolution as supplied unit
  return heading(vex::rotationUnits::rev);
}

double inteldrive::position(vex::rotationUnits units) {
  //gets average of left motor group and right motor group positions in given units
  return (left.position(units) + right.position(units)) / 2.0;
}

double inteldrive::position() {
  //redirects to position(units) with revolutions as supplied unit
  return position(vex::rotationUnits::rev);
}

void inteldrive::resetHeading() {
  //resets heading on inertial sensor
  inertialSensor.resetHeading();
}

void inteldrive::resetPosition() {
  //resets positions on both left and right motor groups
  left .resetPosition();
  right.resetPosition();
}

void inteldrive::drive(double vel, double ratio) {
  //drives left and right motor groups at percent velocity skewed right at ratio
  left .spin(vex::directionType::fwd, vel / ratio, vex::velocityUnits::pct);
  right.spin(vex::directionType::fwd, vel * ratio, vex::velocityUnits::pct);
}

void inteldrive::drive(double vel) {
  //drives left and right motor groups at percent velocity
  left .spin(vex::directionType::fwd, vel, vex::velocityUnits::pct);
  right.spin(vex::directionType::fwd, vel, vex::velocityUnits::pct);
}

void inteldrive::stop(vex::brakeType mode) {
  //stops left and right motor groups with supplied brake type
  left .stop(mode);
  right.stop(mode);
}

void inteldrive::turnTo(double ang, double vel, bool additive) {
  if (!additive) //defaulted to do relative turns
    resetHeading();
  //runs turnPID at angle
  turnPID.run(ang);
  stop(vex::brakeType::brake);
}

void inteldrive::driveTo(double dist, double vel, bool additive) {
  dist *= distanceRatio; //from inches to rotations
  if (!additive) //defaulted to do relative movements
    resetPosition();

  double prevHeading = heading(); //uses heading to keep robot straight
  resetHeading();

  //runs drivePID at distance
  drivePID.run(dist);

  inertialSensor.setHeading(prevHeading, vex::rotationUnits::rev); //reset heading back
  stop(vex::brakeType::brake); 
}

void inteldrive::driveTo(vec2 loc, double vel, bool additive) {
  //breaks vector into polar components and redirects to turnTo and driveTo
  turnTo(loc.ang(), vel, additive);
  driveTo(loc.mag() * distanceRatio, vel, additive);
}

void inteldrive::arcade(double vertical, double horizontal, double vertModifer, double horiModifer) {
  //multiplies the vertical/horizontal with corresponding modifiers
  vertical *= vertModifer;
  horizontal *= horiModifer;
  //spins left and right motor with given vertical/horizontal percents
  left .spin(vex::directionType::fwd, vertical + horizontal, vex::percentUnits::pct);
  right.spin(vex::directionType::fwd, vertical - horizontal, vex::percentUnits::pct); 
}

void inteldrive::tank(double l, double r, double modifer) {
  //spins left and right motors with corresponding inputs multiplied by modifer
  left .spin(vex::directionType::fwd, l * modifer, vex::percentUnits::pct);
  right.spin(vex::directionType::fwd, r * modifer, vex::percentUnits::pct);
}

void inteldrive::runLPS() {
  uint32_t dt = 50; //change in time
  uint32_t ptime = vex::timer::system(); //previous time

#if DEBUG == LPS
  //prints out debugging header for LPS
  robot::brain.Screen.printAt(0, 20, "L: "); //Location
#endif

  while (1) { //should get interrupted elsewhere
    location += vec2::polar(position() / distanceRatio, heading());
    
#if DEBUG == LPS
    //prints actual vector corresponding to label above
    robot::brain.Screen.printAt(20, 20, "<%4.4f, %4.4f>", location.x, location.y);
#endif

    vex::this_thread::sleep_until(ptime + dt);
    ptime = vex::timer::system(); //updates previous time to current time
  }
}