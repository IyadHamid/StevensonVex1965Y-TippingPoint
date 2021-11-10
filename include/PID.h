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

  //constructor
  PID(std::function<double(double)> func_e, 
      std::function<void(double)> func_out,
      kPID k
    ) : k{k}, error{func_e}, output{func_out} {}

  void run(double goal, uint32_t dt = 50);
  
protected:
  //keep protected (instead of private) so it can be overrided in child class
  kPID k; //constants for PID equation
  std::function<double(double)> error; //function to calculate error
  std::function<void(double)> output;  //function to run output
};