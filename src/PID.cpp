/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       PID.cpp                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      10 Sep 2020                                               */
/*    Description:  Proportional Integral Derivative controller               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "PID.h"
#include "v5_vcs.h"

void PID::run(double goal) {
  double integral = 0.0;   //integral/sum of all errors
  double e = error(goal); //current error
  double pe = e;          //previous error
  vex::timer timer;       //used to calculate dt
  timer.reset();          //resets timer to read 0

  while (e > tolerance) {
    const double dt = timer.value(); //change in time in seconds
    timer.reset();

    //integral of error = ∫e(t)dt = Σe(t)dt
    integral += e * dt;
    //derivative of error = d/dt * e(t) = (e(t) - e(t-h))/h
    const double derivative = (e - pe) / dt; //change in error relative to time
    //PID = Kp*e(t) + Ki*(∫e(t)dt) + Kd*(d/dt*e(t))
    const double out = kp * func(e) + ki * integral + kd * derivative;

    output(out); //send output value back

    pe = e;
    e = error(goal);
  }
}