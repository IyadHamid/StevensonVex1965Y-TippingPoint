/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.h                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed 15 Sep 2021                                           */
/*    Description:  Declares robot namespace                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include "v5_cpp.h"

#include "inteldrive.h"

//base classes found here https://api.vexcode.cloud/v5/
//ports/settings are set up in config.h
namespace robot {
  //"brain"/computer of the robot
  extern vex::brain brain;
  //primary controller connected
  extern vex::controller primary;


  //left motors
  extern vex::motor lfront, ltop, lback;
  //right motors
  extern vex::motor rfront, rtop, rback;
  
  //lift motor
  extern vex::motor lift;
  //intake motor
  extern vex::motor intake;

  //intelligent drive train
  extern inteldrive idrive;

  //single-acting solenoid attached to a piston controlling the front claw
  extern vex::pneumatics frontClaw;
  extern bool frontClawOpen;
  //double-acting solenoid attached to the pistons controlling the back claw
  extern vex::pneumatics backClaw;
  extern bool backClawOpen;
  
  //init function
  void init();
}