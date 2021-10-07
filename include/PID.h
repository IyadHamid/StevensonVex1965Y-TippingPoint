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

#define DEBUG

//PID controller class 
class PID {
public:
  //constructor
  PID(double p, double i, double d, 
      std::function<double(double)> func_e, 
      std::function<void(double)> func_out,
      std::function<double(double)> func_p,
      double t = .1
    ) : kp{p}, ki{i}, kd{d}, error{func_e}, output{func_out}, func{func_p}, tolerance{t} {}

  void run(double goal);
  
protected:
  //keep protected (instead of private) so it can be overrided in child class
  double kp, ki, kd; //constants
  double tolerance;  //tolerance accepted
  std::function<double(double)> error; //function to calculate error
  std::function<void(double)> output;  //function to run output
  std::function<double(double)> func;  //function for proportional
};