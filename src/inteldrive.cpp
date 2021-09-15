/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       inteldrive.cpp                                            */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Thu Sep 16 2020                                           */
/*    Description:  Definition of inteldrive                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "inteldrive.h"

#include "common.h"
#include "PID.h"

inteldrive::inteldrive(vex::inertial i, double ratio, 
                       vex::motor_group l, vex::motor_group r,
                       double rw)
                      : inchesRatio{ratio}, absoluteLocation{0.0, 0.0}, 
                        inertialSensor{i}, left{l}, right{r},
                        robotWidth{rw} {
}


void inteldrive::drive(vex::directionType dir, double vel, vex::velocityUnits units, double ratio) {
  ratio = sqrt(ratio);
  left .spin(dir, vel / ratio, units);
  right.spin(dir, vel * ratio, units);
}

void inteldrive::drive(vex::directionType dir, double pct, vex::percentUnits units) {
  left .spin(dir, pct, units);
  right.spin(dir, pct, units);
}

void inteldrive::stop(vex::brakeType mode) {
  left .stop(mode);
  right.stop(mode);
}


void inteldrive::turnTo(double a, double vel, vex::velocityUnits units) {
  auto error = [this](double goal) {
    return angle_difference(goal, getYaw());
  };
  auto output = [this, units](double input) {
    left .spin(vex::directionType::fwd, input, units);
    right.spin(vex::directionType::rev, input, units);
  };
  auto func = [vel](double input) {
    return input * vel;
  };

  PID pid(1.0, 1.0, 1.0, error, output, func);
  pid.run(a);
}

void inteldrive::driveTo(vec2 loc, double vel, vex::velocityUnits units) {
  vec2 disp = loc - absoluteLocation;
  turnTo(disp.ang(), vel);
  auto error = [this, loc](double goal)->double {
    return (const_cast<vec2&>(loc) - absoluteLocation).mag();
  };
  auto output = [this, units](double input) {
    drive(vex::directionType::fwd, input, units);
  };
  auto func = [vel](double input) {
    return input * vel;
  };

  PID pid(1.0, 1.0, 1.0, error, output, func);
  pid.run(error(0.0));
}

void inteldrive::arcTo(vec2 loc, double ang, bool cw, double vel) {
  double dist = (absoluteLocation - loc).mag();
  //cos(ang)=(r^2+r^2-d^2)/(2*r*r)
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

  PID pid(1.0, 1.0, 1.0, error, output, func);
  pid.run(error(0.0));
}

double inteldrive::getYaw(vex::rotationUnits units) {
  return inertialSensor.orientation(vex::orientationType::yaw, units);
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