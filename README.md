# **X - Drive**

Stevenson 1965Y 2021-2022

![](./media/stevensonvexlogo.png "Stevenson Robotics 2021-2022")
Competition styled for Vex EDR [Tipping Point](https://www.vexrobotics.com/vexedr/competition/vrc-current-game "VEX Current Game"). Coded with [VEXCode V5 Pro](https://www.vexrobotics.com/vexcode "VEXCode V5") in C++ using the [VEXcode API](https://api.vexcode.cloud/v5/).
Find this code [here](https://www.github.com/IyadHamid/StevensonVex1965Y-TippingPoint "Stevenson 1965Y Repository") on GitHub!

This repository/project also contains a regular log [here](Log.md).

## **The Team:**
- Adam Taylor
- Andrei Nistor
- Iyad Hamid - *Primary Coder*
- Joshua Neela
- Neil Thakker
- Tanmay Panchpor
	
## **Functionality**
- Generic PID controller
- Generic intelligent drivetrain
  - Uses PID controller for precise autonomous control
  - Has controller functions for driver control
- Simple 2D vector
  - A *lot* of vector mathmematics used
- Useful **comments** *everywhere*!
  - And a comprehensive [log](./log.md)!

## **Robot Configuration**
Configured in [config.h](./include/config.h).

| Name               | Type     | Port(s) |
|:-------------------|:---------|:--------|
|`front left drive  `| motor    | 8       |
|`back left drive   `| motor    | 7       |
|`top left drive    `| motor    | 9       |
|`front right drive `| motor    | 19      |
|`back right drive  `| motor    | 5       |
|`top right drive   `| motor    | 18      |
|`claw lift         `| motor    | 13      |
|`back lift         `| motor    | 2       |
|`inertial sensor   `| sensor   | 10      |
|`claw              `| solenoid | F       |
|`hooks             `| solenoid | B       |

![](./media/robot.png "Stevenson Robotics 2021-2022")

## **Controller Configuration**
**TBA**

# *...because it "just works".*