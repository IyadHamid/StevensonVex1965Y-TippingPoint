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

//PID controller class 
class PID {
public:
  
  //structure containing the 3 PID constants (kp, ki, kd) and tolerance
  struct kPID {
    double p, i, d, t;
  };

  // constructor; error input function, output function, PID constants
  PID(std::function<double(double)> func_e, 
      std::function<void(double)> func_out,
      kPID k
    ) : k{k}, error{func_e}, output{func_out} {}

  // runner; goal to achieve, maximum change for output (defaulted to 0, 0 is no maximum), change in time per cycle in ms (defaulted to 50)
  void run(double goal, double dmax = 0.0, uint32_t dt = 50);
  
  //keeps public (instead of private/protected) so it can be changed during runtime
  kPID k; //constants for PID equation
  std::function<double(double)> error; //function to calculate error
  std::function<void(double)> output;  //function to run output
};