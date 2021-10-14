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

#ifdef DEBUG
#include "robot.h"
#endif

void PID::run(double goal) {
  double integral = 0.0;  //integral/sum of all errors
  double e = error(goal); //current error
  double pe = e;          //previous error

  
#ifdef DEBUG
  robot::brain.Screen.printAt(0, 20,  "P: ");
  robot::brain.Screen.printAt(0, 40,  "I: ");
  robot::brain.Screen.printAt(0, 60,  "D: ");
  robot::brain.Screen.printAt(0, 80, "o: ");
#endif

  while (e > tolerance) {
    //integral of error = ∫e(t)dt = Σe(t)dt
    integral += e;
    //derivative of error = d/dt * e(t) = (e(t) - e(t-h))/h
    const double derivative = (e - pe); //change in error
    //PID = Kp*e(t) + Ki*(∫e(t)dt) + Kd*(d/dt*e(t))
    const double out = k.p * e + k.i * integral + k.d * derivative;

#ifdef DEBUG
    robot::brain.Screen.printAt(20, 20, "%4.4f", e);
    robot::brain.Screen.printAt(20, 40, "%4.4f", integral);
    robot::brain.Screen.printAt(20, 60, "%4.4f", derivative);
    robot::brain.Screen.printAt(20, 80, "%4.4f", out);
#endif

    output(out); //send output value back

    pe = e;          //Set previous error
    e = error(goal); //Set new error
  }
}