/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.cpp                                                 */
/*    Author:       Iyad Hamid                                                */
/*    Created:      Wed 15 Sep 2021                                           */
/*    Description:  Defines robot namespace                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "robot.h"

#include <vector>
#include <utility>

#include "config.h"

namespace robot {
  //initalizes brain
  vex::brain brain;
  //initalizes primary controller
  vex::controller primary;

  //initializes all 8 motors with data in config.h
  vex::motor lfront(std::get<0>(left_front ), std::get<1>(left_front ), std::get<2>(left_front ));
  vex::motor ltop  (std::get<0>(left_top   ), std::get<1>(left_top   ), std::get<2>(left_top   ));
  vex::motor lback (std::get<0>(left_back  ), std::get<1>(left_back  ), std::get<2>(left_back  ));
  vex::motor rfront(std::get<0>(right_front), std::get<1>(right_front), std::get<2>(right_front));
  vex::motor rtop  (std::get<0>(right_top  ), std::get<1>(right_top  ), std::get<2>(right_top  ));
  vex::motor rback (std::get<0>(right_back ), std::get<1>(right_back ), std::get<2>(right_back ));
  vex::motor lift  (std::get<0>(lifts      ), std::get<1>(lifts      ), std::get<2>(lifts      ));
  vex::motor intake(std::get<0>(intakes    ), std::get<1>(intakes    ), std::get<2>(intakes    ));

  //initalizes left motor group with left motors
  vex::motor_group lgroup (lfront, ltop, lback);
  //initalizes right motor group with right motors
  vex::motor_group rgroup(rfront, rtop, rback); 

  //initalizes inertial sensor with port from config.h
  vex::inertial isensor(inertial_port);

  bool intakeRunning = false;

  //creates empty inteldrive which gets initalized later in robot::init()
  inteldrive idrive(
    isensor, 
    lgroup, 
    rgroup,
    drive_k, turn_k,
    fast_drive_k, fast_turn_k,
    disp_k,
    inches2units_ratio, robot_width
  );

  //initalizes front claw with triport from config.h
  vex::pneumatics frontClaw(brain.ThreeWirePort.FRONT_PORT);
  //initalizes back claw with triport from config.h
  vex::pneumatics backClaw(brain.ThreeWirePort.BACK_PORT);
}

void robot::init() {
  //prints if any device is not installed
  std::vector<std::pair<vex::device, std::string>> devices{ 
    { lfront, "left front motor"  }, { ltop, "left top motor"  }, { lback, "left back motor"  }, //left motors
    { rfront, "right front motor" }, { rtop, "right top motor" }, { rback, "right back motor" }, //left motors
    { isensor, "inertial sensor" } //sensors
  };
  for (auto d : devices) {
    if (!d.first.installed()) {
      brain.Screen.print("Warning: %s not installed!", d.second.c_str());
      brain.Screen.newLine();
    }
  }
  std::vector<std::pair<vex::triport::port, std::string>> legacy{
    { brain.ThreeWirePort.FRONT_PORT, "front"} , { brain.ThreeWirePort.BACK_PORT, "back" }
  };

  robot::backClaw.set(false);
  robot::lift.stop(vex::brakeType::hold);
  robot::frontClaw.open();
  robot::backClaw.open();

  isensor.resetHeading();
  idrive.start();
}