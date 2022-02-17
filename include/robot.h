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
  //back motor
  extern vex::motor back;

  //intelligent drive train
  extern inteldrive idrive;

  //single-acting solenoid attached to a piston controlling the claw
  extern vex::pneumatics claw;
  extern bool clawOpen;
  //double-acting solenoid attached to the pistons controlling the each hook
  extern vex::pneumatics backClaw;
  extern bool backClawOpen;
  
  //init function
  void init();

  //sets lift up/down; boolean go up?, wait for completion? (defualted to false)
  void liftSet(bool newState, bool waitForCompletion = false);
  //toggles lift from being up/down
  void liftToggle();
  //moves lift up/down with limits and returns when at a limit; boolean go up?
  bool liftAnalog(bool goUp);

  //sets lift up/down; boolean go up?
  void backSet(bool newState);
  //toggles back from being up/down
  void backToggle();

}