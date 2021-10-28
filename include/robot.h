/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.h                                                   */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed 15 Sep 2021                                           */
/*    Description:  Declares robot namespace                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include "v5.h"
#include "v5_vcs.h"

#include "inteldrive.h"

//classes found here https://api.vexcode.cloud/v5/
//ports/settings are set up in config.h
namespace robot {
  //"brain"/computer of the robot
  extern vex::brain brain;
  //primary controller connected
  extern vex::controller primary;

  //intelligent drive train
  extern inteldrive idrive;

  //left motors
  extern vex::motor lfront, lmiddle, lback;
  //right motors
  extern vex::motor rfront, rmiddle, rback;
}