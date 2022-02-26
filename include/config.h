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

// admin testing controls
#define ADMIN

// auton type
#define AUTON_A

const int32_t inertial_port = PORT10;

// { port, gear ratio, is reverse? }
const motor_settings left_front  { PORT13, gearSetting::ratio6_1, false };
const motor_settings left_back   { PORT16, gearSetting::ratio6_1, false };
const motor_settings left_top    { PORT17, gearSetting::ratio6_1, false };

const motor_settings right_front { PORT7 , gearSetting::ratio6_1, true  };
const motor_settings right_back  { PORT8 , gearSetting::ratio6_1, true  };
const motor_settings right_top   { PORT9 , gearSetting::ratio6_1, true  };

const motor_settings lifts   { PORT14, gearSetting::ratio36_1, true  };
const motor_settings intakes { PORT6 , gearSetting::ratio6_1 , false };

constexpr double inches2units_ratio = 1.0/(pi * 3.25) * 36.0/24.0; // 1/πd * [ratio] (revolutions/inches), ~6.1 inches per rev
constexpr double robot_width = 12.0;

// { proportional constant, integral constant, derivative constant, tolerance }
constexpr kPID drive_k      = { 10.0, 5.0, 5.0, 0.1 };
constexpr kPID fast_drive_k = { 14.0, 6.5, 6.0, 0.1 };

constexpr kPID turn_k       = { 50.0, -10.0, -10.0, 0.001 };
constexpr kPID fast_turn_k  = { 105.0, -32.0, -30.0, 0.005 };

#define FRONT_PORT F
#define BACK_PORT  H

constexpr double lift_back   = -1.2;
constexpr double lift_center =  0.0;
constexpr double lift_front  =  1.75;

constexpr double lift_ring_thresh = 0.25;