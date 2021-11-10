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

// intelligent drivetrain
//drivetrain which uses inertial sensor and PID for fine movement
class inteldrive {
public:
  vex::inertial inertialSensor; // vex v5 inertial sensor
  vex::motor_group left, right; // vex v5 left/right motor groups

  // inteldrive constructor; inertial sensor, left drive train motor group, right drive train motor group,
  //drive PID constants, turn PID constants,
  //ratio revolutions/inches, robot width in inches
  inteldrive(vex::inertial i, vex::motor_group l, vex::motor_group r, 
             PID::kPID drive_k, PID::kPID turn_k, 
             double ratio, double robotWidth);
  ~inteldrive() {}
  
  // gyrometer's heading; units for returned rotation
  double heading(vex::rotationUnits units);
  // gyrometer's heading in revolutions
  double heading();
  // motor encoder's rotational; units for returned rotation
  double position(vex::rotationUnits unit);
  // motor encoder's rotational position in revolutions
  double position();

  // resets inertial sensor's heading to 0 revolutions
  void resetHeading();
  // resets motor encoder's to 0 revolutions
  void resetPosition();

  // drives at velocity with turning ratio; percent velocity, ratio for left : right
  void drive(double vel, double ratio);
  // drives at velocity; percent velocity
  void drive(double vel);
  // stops motors; brake type (defaulted to braking)
  void stop(vex::brakeType mode = vex::brakeType::brake);

  // turns robot to angle; revolution angle, percent velocity, should not reset gyrometer? (defaulted to false)
  void turnTo(double ang, double vel, bool additive = false);
  // drives robot to distance; inches distance, percent velocity, should not reset motor encoders? (defaulted to false)
  void driveTo(double dist, double vel, bool additive = false);
  // drives robot to displacement; 2D vector inches displacement, percent velocity, should not reset motor encoders/gyrometer? (defaulted to false)
  void driveTo(vec2 loc, double vel, bool additive = false);
  // currently non-functional
  void arcTo(vec2 loc, double ang, bool cw, double vel);

  // arcade style drive controls; vertical percent, horizontal percent, percent vertical modifer (defaulted to 1), percent horizontal modifer (defaulted to 1)
  void arcade(double vertical, double horizontal, double vertModifier = 1.0, double horiModifer = 1.0);
  // tank style drive controls; left percent, right percent, percent modifer (defaulted to 1)
  void tank(double l, double r, double modifer = 1.0);

protected:
  PID drivePID, turnPID; // PID constants

  const double robotWidth; // robot width in distance units
  double distanceRatio; // ratio from distance units to revolutions (revolutions/distance units)
};
