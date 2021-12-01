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

  //solenoid attached to piston controlling the claw
  extern vex::pneumatics claw;
  
  //init function
  void init();

  //sets lift up
  void liftUp();
  //sets lift down
  void liftDown();
  //toggles back from being up/down
  void backToggle();

}