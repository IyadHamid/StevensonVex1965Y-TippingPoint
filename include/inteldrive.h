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

  inteldrive(vex::inertial i, vex::motor_group l, vex::motor_group r, 
             PID::kPID drive_k, PID::kPID turn_k, 
             double ratio, double robotWidth, 
             vex::velocityUnits vUnits);
  ~inteldrive() {}
  
  double heading(vex::rotationUnits units);
  double heading();
  double position(vex::rotationUnits unit);
  double position();

  void resetHeading();
  void resetPosition();

  void drive(vex::directionType dir, double vel, double ratio);
  void drive(vex::directionType dir, double vel);
  void stop(vex::brakeType mode = vex::brakeType::brake);

  void turnTo(double ang, double vel, bool additive = false);
  void driveTo(double dist, double vel, bool additive = false);
  void driveTo(vec2 loc, double vel, bool additive = false);
  void arcTo(vec2 loc, double ang, bool cw, double vel);

  void arcade(double vertical, double horizontal, double vertModifier = 1.0, double horiModifer = 1.0);
  void tank(double l, double r, double modifer = 1.0);

  const vex::velocityUnits velUnits;

protected:
  PID drivePID, turnPID;

  const double robotWidth; //robot width in inches
  double inchesRatio; //ratio from inches to radians
};
