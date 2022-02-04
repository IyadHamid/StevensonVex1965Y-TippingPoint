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
                       PID<>::kPID drive_k, PID<>::kPID turn_k,
                       double ratio, double rw)
: inertialSensor{i}, left{l}, right{r},
  location{0.0, 0.0},
  drivePID(), turnPID(),
  robotWidth{rw}, distanceRatio{ratio}
{
  drivePID.k = drive_k;
  turnPID.k = turn_k;
}

void inteldrive::start() {
  //callibrates inertial sensors
  inertialSensor.calibrate();
  //waits until it is done calibrating
  while (inertialSensor.isCalibrating())
    vex::this_thread::sleep_for(100); //sleeps to save cpu resources
  
  drivePID = PID<>( //initalizes drivePID
    [&](double goal) { return goal - position(); },
    [&](double output, double goal) { drive(output); }, 
    drivePID.k
  );

  turnPID = PID<>( //initalizes turnPID
    [&](double goal) { return angle_difference_rev(goal, heading()); }, //turnPID internally uses radians
    [&](double output, double goal) {
      left .spin(vex::directionType::fwd, output * 150.0, vex::voltageUnits::mV);
      right.spin(vex::directionType::rev, output * 150.0, vex::voltageUnits::mV);
    },
    turnPID.k
  );

  dispPID = PID<vec2>(
    [&](vec2 goal){
      // displacement between target location and target location
      auto disp = goal - getLocation();
      // relative angle towards center of circle (90° from angle between heading at destination)
      const auto ang = angle_difference_rad(pi / 2.0, disp.ang() - rev2rad(heading()));
      // driving radius
      const auto r = disp.mag() / (2.0 * cos(ang));
      // arclength distance
      const auto dist = r * 2.0 * ang;
      return dist;
    },
    [&](double output, vec2 goal){
      // displacement between target location and target location
      auto disp = goal - getLocation();
      // relative angle towards center of circle (90° from angle between heading at destination)
      const auto ang = angle_difference_rad(pi / 2.0, disp.ang() - rev2rad(heading()));
      // ratio between left and right wheels
      const auto ratio = 0.5 + disp.mag() / (2.0 * robotWidth * cos(ang));
      drive(output, ratio);
    },
    { 1.0, 1.0, 1.0, 1.0 }//dispPID.k
  );
  
  
  //Starts tracking
  location = {0.0, 0.0};
  trackingThread = CREATE_METHOD_THREAD( inteldrive, locationTrack() );
  trackingThread.setPriority(vex::thread::threadPriorityHigh);
}

double inteldrive::heading(vex::rotationUnits units) {
  //redirects to inertial sensor
  return inertialSensor.heading(units);
}

double inteldrive::position(vex::rotationUnits units) {
  //gets average of left motor group and right motor group positions in given units
  return (left.position(units) + right.position(units)) / 2.0;
}

void inteldrive::drive(double vel, double ratio) {
  if (vel == 0.0) {
    stop();
    return;
  }
  //drives left and right motor groups at percent velocity skewed right at ratio
  left .spin(vex::directionType::fwd, vel / ratio * 150.0, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, vel * ratio * 150.0, vex::voltageUnits::mV);
}

void inteldrive::drive(double vel) {
  if (vel == 0.0) {
    stop();
    return;
  }
  //drives left and right motor groups at percent velocity
  left .spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
  right.spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
}

void inteldrive::stop(vex::brakeType mode) {
  //stops left and right motor groups with supplied brake type
  left .stop(mode);
  right.stop(mode);
}

void inteldrive::turnTo(double ang, uint32_t timeout, double vel, bool relative) {
  if (relative) //defaulted to do relative turns
    ang += heading();
  //runs turnPID at angle
  turnPID.run(ang, timeout, vel);
  stop();
}

void inteldrive::driveTo(double dist, uint32_t timeout, double vel, bool relative) {
  dist *= distanceRatio; //from inches to rotations
  if (relative) //defaulted to do relative movements
    dist += position();

  //runs drivePID at distance
  drivePID.run(dist, timeout, vel);
  stop(); 
}

void inteldrive::driveTo(vec2 loc, uint32_t timeout, double vel, bool relative) {
  //if (relative)
  //  loc -= location;
  //
  //dispPID.run(loc);
  if (!relative)
    loc -= location;
  turnPID.run(loc.ang());
  drivePID.run(loc.mag());
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

void inteldrive::reset() {
  location = { 0.0, 0.0 };
  inertialSensor.resetHeading();
  left.resetPosition();
  right.resetPosition();
}

vec2 inteldrive::getLocation() {
  return location;
}

void inteldrive::locationTrack() {
  deltaTracker<double> dist([&]{ return position(); });
  deltaTracker<double> dir ([&]{ return heading();  });
  
  while (1)  {
    const auto distance = ++dist / getDistanceRatio();
    const auto angle = rev2rad(heading());
    location += vec2::polar(distance, angle);

    vex::this_thread::sleep_for(50); //sleeps to lower cpu usage
  }
}
