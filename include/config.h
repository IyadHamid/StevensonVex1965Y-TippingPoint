/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       config.h                                                  */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed Oct 27 2021                                           */
/*    Description:  Configuration used for robot                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <tuple>
#include "v5_cpp.h"

#include "common.h"
#include "PID.h"

using namespace vex;
typedef std::tuple<int32_t, gearSetting, bool> motor_settings;

// type of debugging DEBUG_PID
#define DEBUG_PID

// { forward controller modifier, right controller modifer }
constexpr vec2 controller_modifiers { 0.4, 0.4 };
// type of drive [ARCADE/DRONE/TANK] (defaults to TANK)
#define DRONE

// auton type
#define AUTON_A

const int32_t inertial_port = PORT10;

// { port, gear ratio, is reverse? }
const motor_settings left_front  { PORT8 , gearSetting::ratio6_1, true  };
const motor_settings left_back   { PORT7 , gearSetting::ratio6_1, true  };
const motor_settings left_top    { PORT9 , gearSetting::ratio6_1, false };

const motor_settings right_front { PORT19, gearSetting::ratio6_1, false };
const motor_settings right_back  { PORT5 , gearSetting::ratio6_1, false };
const motor_settings right_top   { PORT18, gearSetting::ratio6_1, true  };

const motor_settings claw_lift   { PORT13, gearSetting::ratio18_1, false };
const motor_settings back_lift   { PORT2 , gearSetting::ratio18_1, false };

constexpr double inches2units_ratio = 1.0/(pi * 3.25) * 60.0/36.0; // 1/πd * [ratio] (revolutions/inches), ~6.1 inches per rev
constexpr double robot_width = 12.0;

// { proportional constant, integral constant, derivative constant, tolerance }
constexpr PID::kPID drive_k = { 15.0, 8.0, 10.0, 0.1 };
constexpr PID::kPID turn_k  = { 115.0, 15.0, 20.0, 0.005 };

#define CLAW_PORT F
#define HOOKS_PORT B

constexpr double back_up = 0.0;
constexpr double back_down = 1.950;
constexpr double back_high = 0.0;

constexpr double lift_up = 4.570;
constexpr double lift_down = 0.0;
