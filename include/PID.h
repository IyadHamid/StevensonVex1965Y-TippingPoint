/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       PID.h                                                     */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Thu 10 Sep 2020                                           */
/*    Description:  Proportional integral derivative controller header        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <functional>
#include <cmath>

#include "deltaTracker.h"

//structure containing the 3 PID constants (kp, ki, kd) and tolerance
struct kPID {
  double p, i, d, t;
};

//PID controller class
template<typename T = double>
class PID {
public:
  

  PID() {};
  // constructor; error input function, output function, PID constants
  PID(std::function<double(T)> func_e, std::function<void(double, T)> func_out, kPID k
  ) : k{k}, error{func_e}, output{func_out} {}

  // runner; goal to achieve, timeout (0 is no timeout) maximum change for output (defaulted to 0, 0 is no maximum), change in time per cycle in ms (defaulted to 50)
  void run(T goal, uint32_t timeout = 0, double max = 0.0, uint32_t dt = 50) ;
  
  //keeps public (instead of private/protected) so it can be changed during runtime
  kPID k; //constants for PID equation
  std::function<double(T)> error; //function to calculate error
  std::function<void(double, T)> output;  //function to run output
};

template <typename T>
void PID<T>::run(T goal, uint32_t timeout, double max, uint32_t dt) {
  const auto start = vex::timer::system(); // start time (used for timeout)

  double integral = 0.0;                                   // integral/sum of all errors
  deltaTracker<double> e([&, goal]{ return error(goal);}); // tracks error and derivative of error
  uint32_t now;                                            // current time

  while (std::abs(e.value) > k.t) {
    now = vex::timer::system();

    //smooths integral to 'forget' past used to not overcompensate
    integral /= 2.0;
    //integral of error = ∫e(t)dt = Σe(t)dt
    integral += e.value;
    
    //PID = Kp*e(t) + Ki*(∫e(t)dt) + Kd*(d/dt*e(t))
    const auto out = k.p * e.value + k.i * integral + k.d * e.delta;
    
    //send output value back with maximum at max if max > 0.0
    output(max > 0.0 ? std::min(out, max) : out, goal); 

    ++e; //updates error and derivative
    
    vex::this_thread::sleep_until(now + dt);
    now = vex::timer::system(); //updates previous time to current time

    if (timeout != 0 && ((now - start) > timeout))
      break;
  }
}