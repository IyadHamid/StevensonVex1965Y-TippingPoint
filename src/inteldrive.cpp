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
                       double ratio, double rw,
                       vex::velocityUnits vUnits)
: inertialSensor{i}, left{l}, right{r}, velUnits {vUnits},
  drivePID{ //initalizes drivePID
    [this](double goal) { return goal - position();},
    [this](double input) { drive(vex::directionType::fwd, input); }, 
    drive_k
  },
  turnPID{ //initalizes turnPID
    [this](double goal) { return angle_difference_rad(goal, heading()); }, //turnPID internally uses radians
    [this](double input) {
      left .spin(vex::directionType::fwd, input, velUnits);
      right.spin(vex::directionType::rev, input, velUnits);
    },
    turn_k
  },
  robotWidth{rw}, inchesRatio{ratio}
{
  inertialSensor.calibrate();
  while (inertialSensor.isCalibrating());
}

double inteldrive::heading(vex::rotationUnits units) {
  return inertialSensor.orientation(vex::orientationType::yaw, units);
}

double inteldrive::heading() {
  return rev2rad(inertialSensor.orientation(vex::orientationType::yaw, vex::rotationUnits::rev));
}

double inteldrive::position(vex::rotationUnits units) {
  return (left.position(units) + right.position(units)) / 2.0;
}

double inteldrive::position() {
  return rev2rad(left.position(vex::rotationUnits::rev) + right.position(vex::rotationUnits::rev)) / 2.0;
}

void inteldrive::resetHeading() {
  inertialSensor.resetHeading();
}

void inteldrive::resetPosition() {
  left .resetPosition();
  right.resetPosition();
}

void inteldrive::drive(vex::directionType dir, double vel, double ratio) {
  ratio = sqrt(ratio);
  left .spin(dir, vel / ratio, velUnits);
  right.spin(dir, vel * ratio, velUnits);
}

void inteldrive::drive(vex::directionType dir, double vel) {
  left .spin(dir, vel, velUnits);
  right.spin(dir, vel, velUnits);
}

void inteldrive::stop(vex::brakeType mode) {
  left .stop(mode);
  right.stop(mode);
}

void inteldrive::turnTo(double ang, double vel, bool additive) {
  if (!additive)
    resetHeading();
  turnPID.run(ang);
  stop(vex::brakeType::brake);
}

void inteldrive::driveTo(double dist, double vel, bool additive) {
  dist *= inchesRatio; //From inches to rotations to radians
  if (!additive)
    resetPosition();
  
  drivePID.run(dist);
  //vex::task::sleep(100);
  //drivePID.run(dist / inchesRatio); //Likely temporary
  stop(vex::brakeType::brake);
}

void inteldrive::driveTo(vec2 loc, double vel, bool additive) {
  turnTo(loc.ang(), vel, additive);
  driveTo(loc.mag() * inchesRatio, vel, additive);
}

void inteldrive::arcTo(vec2 loc, double ang, bool cw, double vel) {
//  double dist = (absoluteLocation - loc).mag();
//  //cos(ang)=(r^2+r^2-d^2)/(2*r^2)
//  //r^2=-d^2/(2*cos(ang)-2)
//  double radius = sqrt((-dist*dist)/(2*cos(ang)-2));
//  radius *= cw ? 1.0 : -1.0;
//  double r = (radius-(robotWidth/2.0))*ang;
//  double l = (radius+(robotWidth/2.0))*ang;
//  double ratio = cw ? r/l : l/r;
//
//  double iang = (180.0-ang)/2.0 + (loc - absoluteLocation).ang();
//  //vec2 center = absoluteLocation + vec2{cos(iang) * radius, sin(iang) * radius};
//  turnTo(-1/iang, vel);
//
//  std::function<double(double)> error = [this, loc](double goal)->double {
//    return (const_cast<vec2&>(loc) - absoluteLocation).mag();
//  };
//  std::function<void(double)> output = [this, ratio](double input) { 
//    drive(vex::directionType::fwd, input, vex::velocityUnits::rpm, ratio);
//  };
//  std::function<double(double)> func = [vel](double input) {
//    return input * vel;
//  };
//
//  PID pid(arc_kp, arc_ki, arc_kd, error, output, func);
//  pid.run(error(0.0));
}

void inteldrive::arcade(double vertical, double horizontal, double vertModifer, double horiModifer) {
  vertical *= vertModifer;
  horizontal *= horiModifer;
  left .spin(vex::directionType::fwd, vertical + horizontal, vex::percentUnits::pct);
  right.spin(vex::directionType::fwd, vertical - horizontal, vex::percentUnits::pct); 
}

void inteldrive::tank(double l, double r, double modifer) {
  left .spin(vex::directionType::fwd, l * modifer, vex::percentUnits::pct);
  right.spin(vex::directionType::fwd, r * modifer, vex::percentUnits::pct);
}