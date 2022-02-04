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
    return delta;
  }

  // prefix increment update returning new change
  T operator++() {
    const T old = value;
    value = updater();
    return delta = value - old;
  }
  // postfix increment update adding to last change returning total change
  T operator++(int) {
    const T old = value;
    value = updater();
    return delta += value - old;
  }

  std::function<T(void)> updater; //updater function
  T value; //current value
  T delta; //change in value
};