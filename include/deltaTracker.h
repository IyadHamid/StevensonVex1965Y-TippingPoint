/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       deltaTracker.h                                            */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Thu 13 Jan 2022                                           */
/*    Description:  Delta (change) tracker definition                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <functional>

// delta tracker class
template <typename T>
class deltaTracker {
public:
  // constructor; updater function for value
  deltaTracker(std::function<T(void)> updater) : updater{updater}, value{updater()} {}

  T operator()() {
    return dv;
  }

  // prefix increment update returning new change
  T operator++() {
    const T old = value;
    value = updater();
    return dv = value - old;
  }
  // postfix increment update returning new change
  T operator++(int) {
    return operator++();
  }

  std::function<T(void)> updater; //updater function
  T value; //current value
  T dv;    //change in value
};