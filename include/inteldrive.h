/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       inteldrive.h                                              */
/*    Author:       Iyad H                                                    */
/*    Created:      Thu Sep 16 2020                                           */
/*    Description:  Declaration of inteldrive                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <vector>
#include "v5.h"
#include "v5_vcs.h"

#include "common.h"


class inteldrive {
public:
  vex::inertial inertialSensor;
  vex::motor_group left, right;

  inteldrive(vex::inertial i, double ratio, vex::motor_group l, vex::motor_group r, double robotWidth);
  ~inteldrive() {}
  
  double heading(vex::rotationUnits units = vex::rotationUnits::deg);
  double position(vex::rotationUnits units = vex::rotationUnits::deg);
  void resetHeading();
  void resetPosition();

  void drive(vex::directionType dir, double vel, vex::velocityUnits units, double ratio = 1.0);
  void drive(vex::directionType dir, double pct, vex::percentUnits units = vex::percentUnits::pct);
  void stop(vex::brakeType mode = vex::brakeType::brake);

  void turnTo(double ang, double vel, vex::velocityUnits units = vex::velocityUnits::pct, bool additive = false);
  void driveTo(double dist, double vel, vex::velocityUnits units = vex::velocityUnits::pct, bool additive = false);
  void driveTo(vec2 loc, double vel, vex::velocityUnits units = vex::velocityUnits::pct, bool additive = false);
  void arcTo(vec2 loc, double ang, bool cw, double vel);

  void arcade(double vertical, double horizontal, double vertModifier = 1.0, double horiModifer = 1.0);
  void tank(double l, double r, double modifer = 1.0);

protected:
  const double robotWidth;
  double inchesRatio;
};
