/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       PID.cpp                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      10 Sep 2020                                               */
/*    Description:  Proportional Integral Derivative controller               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "PID.h"

#include "v5_cpp.h"

#include "config.h"

#if DEBUG == PID
#include "robot.h"
#endif

void PID::run(double goal, uint32_t timeout, double dmax, uint32_t dt) {
  const auto start = vex::timer::system(); //start time (used for timeout)

  double integral = 0.0;  //integral/sum of all errors
  double e = error(goal); //current error
  double pe = e;          //previous error
  uint32_t now;           //current time

#if DEBUG == PID
  robot::brain.Screen.clearScreen();
  //prints out debugging values for PID
  robot::brain.Screen.printAt(0, 20, "P: ");       //proportional component
  robot::brain.Screen.printAt(0, 40, "I: ");       //integral component
  robot::brain.Screen.printAt(0, 60, "D: ");       //derivative component
  robot::brain.Screen.printAt(0, 80, "o: ");       //final output
  robot::brain.Screen.printAt(20, 20, "%4.4f", e); //error
#endif

  now = vex::timer::system();
  while (std::abs(e) > k.t || (timeout != 0 && now - start > timeout )) {

    //smooths integral to 'forget' past used to not overcompensate
    integral /= 2.0;
    //integral of error = ∫e(t)dt = Σe(t)dt
    integral += e;
    //derivative of error = d/dt * e(t) = (e(t) - e(t-h))/h
    const double derivative = (e - pe); //change in error
    //PID = Kp*e(t) + Ki*(∫e(t)dt) + Kd*(d/dt*e(t))
    double out = k.p * e + k.i * integral + k.d * derivative;

#if DEBUG == PID
    //prints actual number corresponding to label above
    robot::brain.Screen.printAt(20, 20, "%4.4f", e);
    robot::brain.Screen.printAt(20, 40, "%4.4f", integral);
    robot::brain.Screen.printAt(20, 60, "%4.4f", derivative);
    robot::brain.Screen.printAt(20, 80, "%4.4f", out);
#endif

    if (dmax > 0.0 ) //if dmax > 0, max out to dmax
      out = std::min(out, dmax);
    output(out); //send output value back

    pe = e;          //set previous error
    e = error(goal); //set new error

    vex::this_thread::sleep_until(now + dt);
    now = vex::timer::system(); //updates previous time to current time
  }
}