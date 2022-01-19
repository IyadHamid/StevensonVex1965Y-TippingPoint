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
#include "deltaTracker.h"

inteldrive::inteldrive(vex::inertial i, 
                       vex::motor_group l, vex::motor_group r,
                       PID::kPID drive_k, PID::kPID turn_k,
                       double ratio, double rw)
: inertialSensor{i}, left{l}, right{r},
  drivePID(),
  turnPID(),
  robotWidth{rw}, distanceRatio{ratio}
{
  drivePID.k = drive_k;
  turnPID.k = turn_k;

  //callibrates inertial sensors
  inertialSensor.calibrate();
  //waits until it is done calibrating
  while (inertialSensor.isCalibrating())
    vex::this_thread::sleep_for(100); //sleeps to save cpu resources
  
  trackingThread = MEMBER_FUNCTION_THREAD( inteldrive, locationTrack() );
  trackingThread.setPriority(vex::thread::threadPriorityHigh);
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

double inteldrive::position(vex::rotationUnits units) {
  //gets average of left motor group and right motor group positions in given units
  return (left.position(units) + right.position(units)) / 2.0;
}

void inteldrive::drive(double vel, double ratio) {
  //drives left and right motor groups at percent velocity skewed right at ratio
  left .spin(vex::directionType::fwd, vel / ratio * 150.0, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, vel * ratio * 150.0, vex::voltageUnits::mV);
}

void inteldrive::drive(double vel) {
  //drives left and right motor groups at percent velocity
  left .spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
}

void inteldrive::stop(vex::brakeType mode) {
  //stops left and right motor groups with supplied brake type
  left .stop(mode);
  right.stop(mode);
}

void inteldrive::turnTo(double ang, double vel, bool relative) {
  //calls recapture once
  __attribute__((unused)) static bool once = [&](){ recapture(); return true; }();

  if (relative) //defaulted to do relative turns
    ang += heading();
  //runs turnPID at angle
  turnPID.run(ang, 0, vel);
  stop(vex::brakeType::brake);
}

void inteldrive::driveTo(double dist, double vel, bool relative) {
  //calls recapture once
  __attribute__((unused)) static bool once = [&](){ recapture(); return true; }();

  dist *= distanceRatio; //from inches to rotations
  if (relative) //defaulted to do relative movements
    dist += position();

  //runs drivePID at distance
  drivePID.run(dist, 0, vel);

  stop(vex::brakeType::brake); 
}

void inteldrive::arcade(double vertical, double horizontal, double vertModifer, double horiModifer) {
  //multiplies the vertical/horizontal with corresponding modifiers
  vertical *= vertModifer;
  horizontal *= horiModifer;
  //spins left and right motor with given vertical/horizontal percents
  
  left .spin(vex::directionType::fwd, (vertical + horizontal) * 150, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, (vertical - horizontal) * 150, vex::voltageUnits::mV); 
}

void inteldrive::tank(double l, double r, double modifer) {
  //spins left and right motors with corresponding inputs multiplied by modifer
  left .spin(vex::directionType::fwd, l * modifer * 150.0, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, r * modifer * 150.0, vex::voltageUnits::mV);
}

void inteldrive::driveInPolygon(double dist, int sides) {
  const double ang = 1.0/(double)sides;
  for (double i = 0.0; i < sides; i++) {
    driveTo(dist);
    turnTo(ang);
  }
}

double inteldrive::getDistanceRatio() {
  return distanceRatio;
}

vec2 inteldrive::getLocation() {
  return location;
}

void inteldrive::locationTrack() {
  deltaTracker<double> dist([&](){ return position(); });
  deltaTracker<double> dir ([&](){ return heading();  });

  while (1)  {
    location += vec2::polar(++dist / distanceRatio, rev2rad(heading()));
    vex::this_thread::sleep_for(50); //sleeps to lower cpu usage
  }
}

//used to avoid memory permission error by recapturing this
void inteldrive::recapture() {
  drivePID = PID( //initalizes drivePID
    [&](double goal) { return goal - position(); },
    [&](double input) { drive(input); }, 
    drivePID.k
  );
  turnPID = PID( //initalizes turnPID
    [&](double goal) { return angle_difference_rev(goal, heading()); }, //turnPID internally uses radians
    [&](double input) {
      left .spin(vex::directionType::fwd, input * 150.0, vex::voltageUnits::mV);
      right.spin(vex::directionType::rev, input * 150.0, vex::voltageUnits::mV);
    },
    turnPID.k
  );
}