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

inteldrive::inteldrive(vex::inertial i, 
                       vex::motor_group l, vex::motor_group r,
                       PID::kPID drive_k, PID::kPID turn_k,
                       double ratio, double rw)
: inertialSensor{i}, left{l}, right{r},
  drivePID{ //initalizes drivePID
    [this](double goal) { return goal - position(); },
    [this](double input) { drive(input, angle_difference_rev(0, heading()) + 1.0); }, 
    drive_k
  },
  turnPID{ //initalizes turnPID
    [this](double goal) { return angle_difference_rev(goal, heading()); }, //turnPID internally uses radians
    [this](double input) {
      left .spin(vex::directionType::fwd, input, vex::velocityUnits::pct);
      right.spin(vex::directionType::rev, input, vex::velocityUnits::pct);
    },
    turn_k
  },
  robotWidth{rw}, distanceRatio{ratio}
{
  //callibrates inertial sensors
  inertialSensor.calibrate();
  //waits until it is done calibrating
  while (inertialSensor.isCalibrating())
    vex::task::sleep(100); //sleeps to save cpu resources
}

double inteldrive::heading(vex::rotationUnits units) {
  //redirects to inertial sensor
  return inertialSensor.orientation(vex::orientationType::yaw, units);
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

void inteldrive::arcTo(vec2 loc, double ang, bool cw, double vel) {
  //TBD, math is unconfirmed to work
/*  
  double dist = (absoluteLocation - loc).mag();
  //cos(ang)=(r^2+r^2-d^2)/(2*r^2)
  //r^2=-d^2/(2*cos(ang)-2)
  double radius = sqrt((-dist*dist)/(2*cos(ang)-2));
  radius *= cw ? 1.0 : -1.0;
  double r = (radius-(robotWidth/2.0))*ang;
  double l = (radius+(robotWidth/2.0))*ang;
  double ratio = cw ? r/l : l/r;

  double iang = (180.0-ang)/2.0 + (loc - absoluteLocation).ang();
  //vec2 center = absoluteLocation + vec2{cos(iang) * radius, sin(iang) * radius};
  turnTo(-1/iang, vel);

  std::function<double(double)> error = [this, loc](double goal)->double {
    return (const_cast<vec2&>(loc) - absoluteLocation).mag();
  };
  std::function<void(double)> output = [this, ratio](double input) { 
    drive(vex::directionType::fwd, input, vex::velocityUnits::rpm, ratio);
  };
  std::function<double(double)> func = [vel](double input) {
    return input * vel;
  };

  PID pid(arc_kp, arc_ki, arc_kd, error, output, func);
  pid.run(error(0.0));
*/
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