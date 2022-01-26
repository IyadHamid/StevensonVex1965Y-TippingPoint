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
#include "v5_cpp.h"

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
             PID<>::kPID drive_k, PID<>::kPID turn_k, 
             double ratio, double robotWidth);
  // inteldrive constructor (creates empty inteldrive)
  //inteldrive();
  // inteldrive destructor (does nothing)
  ~inteldrive() {}

  void start();

  // gyrometer's heading; units for returned rotation (defaulted to revolution)
  double heading(vex::rotationUnits units = vex::rotationUnits::rev);
  // motor encoder's rotational; units for returned rotation (defaulted to revolution)
  double position(vex::rotationUnits unit = vex::rotationUnits::rev);

  // drives at velocity with turning ratio; percent velocity, ratio for right / left
  void drive(double vel, double ratio);
  // drives at velocity; percent velocity
  void drive(double vel);
  // stops motors; brake type (defaulted to braking)
  void stop(vex::brakeType mode = vex::brakeType::brake);

  // turns robot to angle; revolution angle, percent velocity (defaulted to 0, 0 is no maximum), should not reset gyrometer? (defaulted to false)
  void turnTo(double ang, double vel = 0.0, bool relative = true);
  // drives robot to distance; inches distance, percent velocity (defualted to 0, 0 is no maximum), should not reset motor encoders? (defaulted to false)
  void driveTo(double dist, double vel = 0.0, bool relative = true);
  // drives robot to distance; inches distance, percent velocity (defualted to 0, 0 is no maximum), should not reset motor encoders? (defaulted to false)
  void driveTo(vec2 loc, double vel = 0.0, bool relative = true);

  // arcade style drive controls; vertical percent, horizontal percent, percent vertical modifer (defaulted to 1), percent horizontal modifer (defaulted to 1)
  void arcade(double vertical, double horizontal, double vertModifier = 1.0, double horiModifer = 1.0);
  // tank style drive controls; left percent, right percent, percent modifer (defaulted to 1)
  void tank(double l, double r, double modifer = 1.0);

  // drive in polygon; distance, sides (minimum 3)
  void driveInPolygon(double dist, int sides);

  // gets distance ratio
  double getDistanceRatio();
  // gets location
  vec2 getLocation();
  
protected:
  // tracks and updates location vector
  void locationTrack();

  vec2 location; // location (when constructor is called)
  vex::thread trackingThread; // thread tracking location

  PID<> drivePID, turnPID; // PID constants

  double robotWidth; // robot width in distance units
  double distanceRatio; // ratio from distance units to revolutions (revolutions/distance units)
};
