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
#include "PID.h"

class inteldrive {
public:
  vex::inertial inertialSensor; //vex v5 inertial sensor
  vex::motor_group left, right; //vex v5 left/right motor groups

  inteldrive(vex::inertial i, vex::motor_group l, vex::motor_group r, PID::kPID drive_k, PID::kPID turn_k, double ratio, double robotWidth);
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
  PID drivePID, turnPID;

  const double robotWidth; //robot width in inches
  double inchesRatio; //ratio from wheel spins to inches
};
