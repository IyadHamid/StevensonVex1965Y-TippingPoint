/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       common.h                                                  */
/*    Author:       Iyad H                                                    */
/*    Created:      Thu Sep 16 2020                                           */
/*    Description:  Common function library                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <cmath>

constexpr double pi = 3.14159265359; //mathmatical π
constexpr double tau = pi * 2.0; //mathmatical τ

// degreees to radians
constexpr double deg2rad(double a) { return a * pi/180.0; }
// degreees to revolutions
constexpr double deg2rev(double a) { return a * 1.0/360.0; }

// radians to degrees
constexpr double rad2deg(double a) { return a * 180.0/pi; }
// radians to revolutions
constexpr double rad2rev(double a) { return a / tau; }

// revolutions to degrees
constexpr double rev2deg(double a) { return a * 360.0; }
// revolutions to radians
constexpr double rev2rad(double a) { return a * tau; }

// is A within T of B
constexpr bool within_tolerance(double a, double b, double t) { 
  return b - t <= a && a <= b + t;
}
// is A within X and Y
constexpr bool within_angle_range(double a, double x, double y) {
  return (x < a && a < y) ||
         (a < y && y < x) ||
         (y < x && x < a);
}
// difference of A and B in degrees
//returns positive : A is right of B
static const double angle_difference_deg(double a, double b) {
  return std::fmod((a - b + 180.0), 360.0) - 180.0;
}
// difference of A and B in degrees
//returns positive : A is right of B
static const double angle_difference_rad(double a, double b) {
  return std::fmod((a - b + pi), tau) - pi;
}
// difference of A and B in degrees
//returns positive : A is right of B
static const double angle_difference_rev(double a, double b) {
  return std::fmod((a - b + 0.5), 1.0) - 0.5;
}

// mathmatical 2-dimensional vector
struct vec2 {
  double x, y;
  
  // creates vector from polar coordinates
  static vec2 polar(double mag, double ang) { return vec2{cos(ang) * mag, sin(ang) * mag}; };

  vec2 operator+(vec2 other) { return vec2{x+other.x, y+other.y}; }
  vec2 operator-(vec2 other) { return vec2{x-other.x, y-other.y}; }
  vec2 operator*(double other) { return vec2{x*other, y*other}; }
  
  void operator+=(vec2 other) { *this = *this + other; }
  void operator-=(vec2 other) { *this = *this - other; }

  // gets polar angle
  double ang() { return tan(y / x); }
  // gets magnitude
  double mag() { return sqrt(x*x + y*y); }
};

//got from 1,000,000 year simulation
constexpr int answerToLifeTheUniverseAndEverything = 42;