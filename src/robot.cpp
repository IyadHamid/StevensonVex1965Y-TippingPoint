/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot.h                                                   */
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
  vex::motor lift  (std::get<0>(claw_lift  ), std::get<1>(claw_lift  ), std::get<2>(claw_lift  ));
  vex::motor back  (std::get<0>(back_lift  ), std::get<1>(back_lift  ), std::get<2>(claw_lift  ));

  //initalizes left motor group with left motors
  vex::motor_group lgroup (lfront, ltop, lback);
  //initalizes right motor group with right motors
  vex::motor_group rgroup(rfront, rtop, rback); 

  //initalizes inertial sensor with port from config.h
  vex::inertial isensor(inertial_port);

  //creates empty inteldrive which gets initalized later in robot::init()
  inteldrive idrive;

  //initalizes claw with triport from config.h
  vex::pneumatics claw(brain.ThreeWirePort.CLAW_PORT);
  //initalizes left/right hook with triport from config.h
  vex::pneumatics hook(brain.ThreeWirePort.HOOKS_PORT);
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
    { brain.ThreeWirePort.CLAW_PORT, "claw"} , { brain.ThreeWirePort.HOOKS_PORT, "hook" }
  };
  
  for (auto d : legacy) {

    //if (!d.first.installed()) {
    //  brain.Screen.print("Warning: %s not installed!", d.second.c_str());
    //  brain.Screen.newLine();
    //}
  }
  //prints if controllers are not connected
  if (!primary.installed())
    brain.Screen.print("Warning: primary controller not connected!");
  brain.Screen.newLine();

  
  //initalizes intelligent drivetrain with above sensor, motor groups, and constants from config.h
  idrive = inteldrive(
    isensor, 
    lgroup, 
    rgroup,
    drive_k, turn_k,
    inches2units_ratio, robot_width
  );
}

bool lift_isUp = false; //lift is initally down
bool back_isUp = true; //back is initally up

void robot::liftSet(bool goUp) { 
  //set lift to up if going up
  lift.rotateTo(goUp ? lift_up : lift_down, vex::rotationUnits::rev, 110, vex::velocityUnits::pct, false);
  lift_isUp = goUp;
}

void robot::liftToggle() {
  //set lift to lift_down if is up
  lift.rotateTo(lift_isUp ? lift_down : lift_up, vex::rotationUnits::rev, 110, vex::velocityUnits::pct, false);
  lift_isUp = !lift_isUp;
}

bool robot::liftAnalog(bool goUp) {
  if (goUp && lift.rotation(rotationUnits::rev) < lift_up)
    lift.spin(directionType::fwd, 100, vex::velocityUnits::pct);
  else if (!goUp && lift.rotation(rotationUnits::rev) > lift_down)
    lift.spin(directionType::rev, 100, vex::velocityUnits::pct);
  else {
    lift.stop(vex::brakeType::hold);
    return true;
  }
  return false;
}

void robot::backSet(bool goUp) { 
  //set lift to up if going up
  back.rotateTo(goUp ? back_up : back_down, vex::rotationUnits::rev, 110, vex::velocityUnits::pct, false);
  back_isUp = goUp;
}

void robot::backToggle() {
  //set lift to lift_down if is up
  back.rotateTo(back_isUp ? back_down : back_up, vex::rotationUnits::rev, 110, vex::velocityUnits::pct, false);
  back_isUp = !back_isUp;
}