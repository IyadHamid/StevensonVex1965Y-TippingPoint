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
                       kPID drive_k     , kPID turn_k,
                       kPID fast_drive_k, kPID fast_turn_k,
                       kPID disp_k,
                       double ratio, double rw)
: inertialSensor{i}, left{l}, right{r},
  location{0.0, 0.0},
  drivePID{}, turnPID{},
  drive_k{ drive_k }, fast_drive_k{ fast_drive_k },
  turn_k{ turn_k }, fast_turn_k{ fast_turn_k },
  robotWidth{ rw }, distanceRatio{ ratio }
{
  drivePID.k = drive_k;
  turnPID.k = turn_k;
  dispPID.k = disp_k;
}

void inteldrive::start() {
  //callibrates inertial sensors
  inertialSensor.calibrate();
  //waits until it is done calibrating
  until(!inertialSensor.isCalibrating());
  
  drivePID = PID<>( //initalizes drivePID
    [&](double goal) { return goal - position(); },
    [&](double output, double goal) { drive_percentage(output); }, 
    drivePID.k
  );

  turnPID = PID<>( //initalizes turnPID
    [&](double goal) { return angle_difference_rev(goal, heading()); }, //turnPID internally uses radians
    [&](double output, double goal) {
      left .spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
      right.spin(vex::directionType::rev, output, vex::voltageUnits::volt);
    },
    turnPID.k
  );

  dispPID = PID<vec2>( //initalizes drivePID
    [&](vec2 goal) { 
      vec2 disp = goal - location;
      vec2 dir  = vec2::polar(1.0, rev2rad(heading()));
      //project displacement onto heading
      auto dist = dot(disp, dir);
      return dist; 
    },
    [&](double output, vec2 goal) {
      vec2 disp = goal - location;
      auto ratio = 2.0 * angle_difference_rev(rad2rev(disp.ang()), heading());
      if (fabs(ratio) > 0.5)
        ratio = 2.0 * (angle_difference_rev(heading(), rad2rev(disp.ang())) - .5);

      left .spin(vex::directionType::fwd, output * (.5 + ratio), vex::percentUnits::pct);
      right.spin(vex::directionType::fwd, output * (.5 - ratio), vex::percentUnits::pct);
    }, 
    drivePID.k
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
  //left .spin(vex::directionType::fwd, vel / ratio * 150.0, vex::voltageUnits::mV);
  //right.spin(vex::directionType::fwd, vel * ratio * 150.0, vex::voltageUnits::mV);
  left .spin(vex::directionType::fwd, vel / ratio, vex::velocityUnits::pct);
  right.spin(vex::directionType::fwd, vel * ratio, vex::velocityUnits::pct);
}

void inteldrive::drive(double vel) {
  if (vel == 0.0) {
    stop();
    return;
  }
  //drives left and right motor groups at percent velocity
  //left .spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
  //right.spin(vex::directionType::fwd, vel * 150.0, vex::voltageUnits::mV);
  left .spin(vex::directionType::fwd, vel, vex::velocityUnits::pct);
  right.spin(vex::directionType::fwd, vel, vex::velocityUnits::pct);
}

void inteldrive::drive_percentage(double vel) {
  if (vel == 0.0) {
    stop();
    return;
  }
  //drives left and right motor groups at percent velocity
  left .spin(vex::directionType::fwd, vel, vex::percentUnits::pct);
  right.spin(vex::directionType::fwd, vel, vex::percentUnits::pct);
}

void inteldrive::stop(vex::brakeType mode) {
  //stops left and right motor groups with supplied brake type
  left .stop(mode);
  right.stop(mode);
}

void inteldrive::turnTo(double ang, double maxSpeed, bool relative, uint32_t timeout) {
  if (relative) //defaulted to do relative turns
    ang += heading();
  turnPID.k = fast_turn_k;
  //runs turnPID at angle
  turnPID.run(ang, timeout, maxSpeed);
  stop();
}

void inteldrive::driveTo(double dist, double maxSpeed, bool relative, uint32_t timeout) {
  dist *= distanceRatio; //from inches to rotations
  if (relative) //defaulted to do relative movements
    dist += position();
  drivePID.k = fast_drive_k;

  //runs drivePID at distance
  drivePID.run(dist, timeout, maxSpeed);
  stop(); 
}

void inteldrive::driveTo(vec2 loc, bool reverse, double maxDriveSpeed, double maxTurnSpeed, bool turnAndDrive) {
  if (turnAndDrive) {
    vec2 disp = loc - location;
    double angle = common_mod(rad2rev(disp.ang()) + (reverse ? 0.5 : 0.0), 1.0);
 
    turnTo(angle, maxTurnSpeed, false); //turn absolutely
    
    disp = loc - location; //recalculates displacement
    vex::this_thread::sleep_for(50); //fixes a turn jerk
    driveTo(disp.mag() * (reverse ? -1.0 : 1.0), maxDriveSpeed); //drive relatively
  }
  //else
  //  dispPID.run(loc);
  stop();
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