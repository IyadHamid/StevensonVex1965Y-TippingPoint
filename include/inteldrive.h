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
             kPID drive_k     , kPID turn_k,
             kPID fast_drive_k, kPID fast_turn_k, 
             double ratio, double robotWidth);
  // inteldrive destructor (does nothing)
  ~inteldrive() {}

  void start();

  // gyrometer's heading; units for returned rotation (defaulted to revolution)
  double heading(vex::rotationUnits units = vex::rotationUnits::rev);
  // motor encoder's rotational; units for returned rotation (defaulted to revolution)
  double position(vex::rotationUnits units = vex::rotationUnits::rev);

  // drives at velocity with turning ratio; percent velocity, ratio for right / left
  void drive(double vel, double ratio);
  // drives at velocity; percent velocity
  void drive(double vel);
  // drives at velocity internally using percent; percent velocity
  void drive_percentage(double vel);
  // stops motors; brake type (defaulted to braking)
  void stop(vex::brakeType mode = vex::brakeType::brake);

  // turns robot to angle; revolution angle, use fast PID? (defaulted to true),
  //   is relative? (defaulted to true), timeout in ms (defaulted to 1000, 0 is no timeout)
  void turnTo(double ang, bool fast = true, bool relative = true, uint32_t timeout = 1000);
  // drives robot to distance; inches distance, use fast PID? (defaulted to true),
  //   is relative? (defaulted to true), timeout in ms (defaulted to 0, 0 is no timeout)
  void driveTo(double dist, bool fast = true, bool relative = true, uint32_t timeout = 0);
  // drives robot to position; displacement, use fast PIDs? (defaulted to true), drive reverse? (defaulted to false)
  void driveTo(vec2 loc, bool fast = true, bool reverse = false);

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
  
  // resets location, position, and heading (use sparingly)
  void reset();
  
protected:
  // tracks and updates location vector
  void locationTrack();

  vec2 location; // location (when constructor is called)
  vex::thread trackingThread; // thread tracking location

  PID<> drivePID, turnPID; // PID controllers using scalar error
  kPID drive_k, fast_drive_k, turn_k, fast_turn_k; // PID constants
  //PID<vec2> dispPID; // PID controller with displacement

  double robotWidth; // robot width in distance units
  double distanceRatio; // ratio from distance units to revolutions (revolutions/distance units)
};