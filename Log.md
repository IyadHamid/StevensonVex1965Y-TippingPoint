# 2021  - 2022 Vex 1965Y Coding Log

### 09/09/21
- Copied `PID.h` and `PID.cpp` from [IyadHamid/StevensonVex1965Y-ChangeUp](https://github.com/IyadHamid/StevensonVex1965Y-ChangeUp)
- Edited `PID.h` and `PID.cpp`
  - Added additional comments/descriptions
  - Accounted for time (`dt`) in equations

### 09/15/21
- Created V5 project file
  - Added and edited `main.cpp`, `robot.h`, `robot.cpp`
- Copied `readme.md` (now `README.md`) and `stevensonlogo.png` (now `stevensonvexlogo.png`) from [IyadHamid/StevensonVex1965Y-ChangeUp](https://github.com/IyadHamid/StevensonVex1965Y-ChangeUp)
- Edited `README.md` to be accurate
- Created Github repository [IyadHamid/StevensonVex1965Y-TippingPoint](https://github.com/IyadHamid/StevensonVex1965Y-TippingPoint)
  - Added GNU GPL3 License
- Copied `common.h`, `inteldrive.h`, `inteldrive.cpp` from [IyadHamid/StevensonVex1965Y- ChangeUp](https://github.com/IyadHamid/StevensonVex1965Y-ChangeUp)
- Edited `inteldrive.h` and `inteldrive.cpp`
  - Simplifed by removing untested/unreliable functions
  - Simplified by removing unused data
  - Renamed `leftDrive`/`rightDrive` to `left`/`right`

### 09/22/2021
- Removed all global absolute location tracking in inteldrive
  - Replaced with relative/temporary absolute tracking
- Overloaded `inteldrive::driveTo`
  - Single dimensional input to move forward/backward
- Various changes to `inteldrive`
  - Refactored code to match between header and source
  - Made PID constants to unique constexprs on top of file
- Added few comments to `PID.cpp`
#### Notes:
- Must redo `inteldrive::arcTo` to match `inteldrive::driveTo` and `inteldrive::turnTo`
- Need to recheck/redo math for `inteldrive::arcTo`

### 09/23/2021
- Reordered parameters in `inteldrive` constructor
- Made `Log.txt` to `Log.md`
  - Reformatted Log to markdown
- Added definitions to `robot.cpp`
  - Added `constexpr` variables for constants such as ports and robot 
- Added some functionality to `main.cpp`
  - Added `vex::competition` instance
  - Added `drivercontrol` function
  - Added `autonomous` function
 
### 09/29/2021
- Tested pnuematics control (through `vex::pneumatics`) and confirmed that it works

### 09/30/2021
- Removed `dt` from `PID::run` as error is already relative to time
- Commented `inteldrive::arcTo` function out to build/test curve
- Added testing code to run `inteldrive::driveTo`
- Added comment descriptors for variables in `inteldrive.h`
- Added support for `inchesRatio` in `inteldrive`
  - Now is ratio for inches : spin
  - `constexpr wheel2inches_ratio` now `constexpr inches2wheel_ratio` in `robot.h` to match
#### Testing
- Made an attempt to run `inteldrive::driveTo`
  - Does not work, nothing happens
#### Notes:
- Need to debug and fix `inteldrive::driveTo`

### 10/04/2021
- Added missing `Log.md` to github repository
- Updated `README.md` to mention `Log.md`

### 10/06/2021
- Fixed link formating in `Log.md`
- Added debugging to `PID` enabled by `DEBUG` macro in `PID.h`
- Added new log category to `Log.md` titled `Testing`specifically to `09/30/2021`
#### Testing
- Tested `inteldrive::driveTo`
  - Does not work, found program hangs in `PID`

### 10/07/21
- Made debugging for `PID::run` viewable/function
- Replaced `not` with `!` in `inteldrive.cpp`
 #### Testing
- Isolated problem from above to `vex::motor` constructor
  - Fixed by using `vex::PORT{number}` instead of literals.
  - Found another issue with `PID`/`inteldrive` not updating position error

### 10/13/21
#### Testing
- Made PID for `inteldrive::driveTo` to `PID::run` again after 100 ms
- Found motors do not stop for `inteldrive::driveTo`
  - Changed accordingly
 
### 10/14/21
- Made `pid`s for `inteldrive::driveTo` and `inteldrive::turnTo` to class members `drivePID` and `turnPID`
  - Reasoning: no need to reinitialize same `PID` object for the same purpose
- Removed `func` from `PID`
  - Updated `PID` calls to match
  - Reasoning: `error` should account for change and integral/derivative will linearize
- Defined `struct kPID` in `PID`
  - Used to replace `kp`, `ki`, and `kd` in `PID` and for `PID` users
  - Reasoning: ease of variable sharing
- Added `drive_k` and `turn_k` in `inteldrive` constructor
  - Reasoning: Used to define constants in `robot.cpp` instead of using `kp`, `ki`, `kd`

 ### 10/20/21
- Added object/global units to `inteldrive`
  - Stored in `velUnits` for velocity and `rotUnits` for angles
  - Changed all functions and function calls with units
  - Removed resulting ambiguity between drive by not having `ratio` set to a value
  - Reasoning: units are getting lost with `PID` usage and compacts function declaration
- Fixed `inteldrive::driveTo` not scaling inches to rotations
- Added `deg2rev`, `rad2deg`, `rad2rev`, `rev2deg`, and `rev2rad`
  - Reasoning: `inteldrive` is using multiple rotation units for math/input and requires translation
#### Notes:
- `inteldrive::turnTo` not functional
- `inteldrive::driveTo` and `inteldrive::turnTo` does not implement speed
  - Possibly use `std::max` or multiplication to set speed
  - Possibly require velocity in percent as limits may be inaccurate/nonexistent

### 10/21/21
- Made `inteldrive` fully support `vex::rotationUnits::rev`
- Used `assert` to assert `inteldrive` does not use `vex::rotationUnits::raw`
  - Reasoning: unable to use raw units in math
- Fixed `PID` not using negative error
- `inteldrive` now only uses radians
  - Reverted changes today
  - Reasoning: internally complicates when using degrees/revolutions instead of radians
- Added tolerance `t` to `kPID`
  - Reasoning: forces user to add own tolerance
- Declared `inchesRatio` in `inteldrive` to be `radians : inches`
  - Reasoning: uncomplicates internal code
#### Notes:
- May want to remove `velUnits` from `inteldrive` to match with removal of `rotUnits`
- Should change `integral` in `PID::run` to only be sum of last few iterations
#### Testing 
- `inteldrive::driveTo(double,...)` and `inteldrive::turnTo` now functional
  - `inteldrive::driveTo(vec2,...)` and `inteldrive::arcTo` still NOT functional

### 10/27/21
- Set limit to `integral` in `PID::run` through dividing by `2.0`
  - Reasoning: without limit, integral often gets out of hand and will overpower PID
- Created `config.h`
  - Moved port/setting definition from `robot.cpp`
  - Reasoning: only has settings for readablity
- Created [TODO](###TODO)
  - Reasoning: things to fix described in the `Notes:` section are lost in log

### 10/27/21
- Locked `PID::run` to account for time
  - Reasoning: integral/derivative were not accounting for time and may mess output
- Flashdrive got corrupted
  - Part of yesterday's and today's work got deleted, had to rewrite
- Removed test code from `main`
  - Reasoning: test base got deconstructed, new testing will be done on final base
- Renamed `[left/right]_middle` to `[left/right]_top` in `config.h`
#### Testing
- Experimented with accelerometer of inertial sensor
  - Accelerometer is not accurate/local enough to be useful

### 11/3/21
- Removed `vex::directionType` parameter from `inteldrive::drive`
  - Replicated to function callees
  - Reasoning: direction is implied from sign of `double vel` parameter, cleans up code
#### Notes:
- Github does not format `Log.md` correctly
  - Likely due to space vs tab
#### Testing
- Tested physical base
  - Getting memory access error when using `idrive` from `robot.cpp`
    - Duct-tape fix of creating new `inteldrive` and using that works
    - Unable to fix

### 11/4/21
- Renamed `lmiddle` and `rmiddle` to `ltop` and `rtop` respectively
  - Reasoning: matches what they are called
- Moved `inertial` constructor outside of `inteldrive` constructor in `robot.cpp`
  - Reasoning: fixes possible memory access error like `left` and `right` `motor_group`s
- Added `claw` variable of `vex::pnuematics` in `robot.h`, `robot.cpp`
  - Initialized with `CLAW_PORT` in `config.h`
- Replaced single spaces with double spaces in `Log.md` to fix list formating in Github
- Updated `inches2wheel_ratio` in `config.h`
#### Testing
- Fixed memory access error
  - Discovered issue was in `left` and `right` due to initialization of `motor_group` inside constructor in `robot.cpp`
  - Solved by constructing `motor_group`s outside of `inteldrive`constructor

### 11/10/21
- Added controls to `main.cpp` and `robot.h`/`robot.cpp`
  - Added specific functions to `robot` namespace (TO BE IMPLEMENTED)
- Removed `velUnits` from `inteldrive`
  - Replaced internally with `vex::velocityUnits::pct`
  - Reasoning: matches with `rotUnits` and keeps code cleaner
- Added comments to `inteldrive.h`
- Renamed `inchesRatio` to `distanceRatio` in `inteldrive`
  - Updated comments to match arbitrariness
  - Reasoning: `inteldrive` distance units are not necessarily in inches
#### Notes: 
- Should add controls to `README.md`
  - Should provide wiring diagram and control diagram
- Need to add auto-straightening to `inteldrive::driveTo` to prevent skewed driving
- Need to fix units for `drivePID` and `turnPID` velocities

### 11/11/21
- Added straight-stabilization to `inteldrive::driveTo`
- Added definitions/declarations for `lift` and `back` motors
  - Added `motor_setting`s in `config.h` named `claw_lift` and `back_lift`
- Added additional comments to all files

### 11/17/21
- Added `robot::init()` function
  - Checks if all devices are connected and prints if they are not
- Added empty `inteldrive` constructor
  - Made `incehsRatio` not constant to not delete implicit move constructor
  - Reasoning: allows to redefine object after creation
    - Used to fix issue described below
- Made `PID` member variables public
  - Reasoning: allows functions/constants to be overriden
- Implemented maxmium output to `PID`
- Created `robot::multitask`
  - Reasoning: using lambdas (anonymous functions) instead of creating new functions for multitasking/multithreading is easier
#### Testing
- `inteldrive` constructor causing memory error
  - Determined `inertialSensor` to be causing the issue
  - Issue actually is `vex::task::sleep`
  - Fixed, initalized `idrive` in `robot::init`

### 12/1/21
- Renamed `robot::multitask` to functionally-same class `multithread` and moved to `common.h`
  - Now uses `vex::thread`
  - Uses a protected function to offload `std::function`
- Renamed class `multithread` to `simplethread`
  - Functionality now more like `vex::thread`
  - Manages internal `vex::thread`
- Removed `simplethread` as `vex::thread` accepts lambdas
- Created helper macro `MEMBER_FUNCTION_THREAD`
  - Helps create `vex::thread` to call member functions
- Removed `inteldrive::arcTo`
  - Reasoning: unnecessary/unimplemented
- Debug macro now is assigned to some value
  - Value is what to debug
  - Reasoning: Cannot print all debug information
- Created `inteldrive::runLPS`
  - Prototyped local position system in new thread (under `lpsThread`)
  - Reasoning: drive algorithms can be improved with absolute location tracking

### 12/2/21
- Replaced custom wait algorithm with `vex::this_thread::sleep_until`
  - Reasoning: custom algorithm may hog CPU resources
- Added left/right hook pnuematics declaration/definition/parameters in `robot.h`, `robot.cpp`, and `config.h`

### 12/8/21
- Merged left/right pnuematic hooks together
  - Reasoning: physical robot using 1 double-acting solenoid for both piston-controlled hooks 
- Implemented pneumatic control for `robot::lift` and `robot::hook`
#### Testing
- Tested pnuematic hooks
  - Mechanical issue, control works as expected

### 12/9/21
- Used new lift controller functions in `main.cpp`
- Assigned new ports in `config.h`
- Fixed `Log.md` formatting
- Temporarily disableed `inteldrive::runLPS`
  - Currently unnecessary
#### Testing
- Tested `inteldrive::driveTo` and `inteldrive::turnTo`
  - Experienced memory permission error for PID control
    - Hotfixed with using `robot::idrive` instead of `this` inside lambda function
  - Found adequate PID values
    - drive: kp=12.0, ki=7.0, kd=0.0, tolerance=0.1
    - turn: kp=120.0, ki=25.0, kd=0.05, tolerance=0.01

### 12/10/21
- Added rumble feedback if claw is closed
- Added timeout to `PID`
  - Reasoning: `PID` may fail and get stuck in an infinite loop
- Added analog control with maximums for lift
  - Rumbles controller when at a maximum
#### Testing
- Tested relative coordinate system
  - Works
- Refined PID through driving along a square

### 12/11/21
- `11/4/21` incorrectly labeled as `1/4/21` in `Log.md`, fixed

### 12/12/21 (Competition)
- Added `TEST` macro
  - Toggles testing mode in `main.cpp`
- Fixed spelling/typogoraphic errors
  - `degreees` to `degrees` in `common.h`
  - `mathamatical` to `mathematical` in `common.h`
  - `robot.h` to `robot.cpp` (in the file description) in `robot.cpp`
- Created simple autonomous 
  - Goes forward, picks up one middle mogo, comes back
  - Multithreaded claw clamping down
    - Reasoning: too much waiting if singlethreaded
- Stopped using namespace `vex` in `config.h`
  - Reasoning: when using namespace `robot` 
  - Reverted
    - Reasoning: Vex headers break
- Changed lift control to toggle
  - Changed to two button toggle (L1 up, L2 down)
- Removed `inteldrive::position()` and `inteldrive::heading()` to use overloaded functions with default parameter
  - Reasoning: redundant
- Possibly fixed `angle_difference_rev` through making value from 0 to 1 using decimal part
#### Testing
- `inteldrive::turnTo` not functional after going straight
  - Negative numbers nonfunctional, must use 0 to 1
#### Competition:
- Sudden failure of controller
  - Had to reset controller
- Turned off in middle of match
  - Unknown battery issue
    - Possibly firmware
    - Possibly battery charge
- Auton failure
  - Distances mismatch, likely want to use ultrasonic sensor for calibration
- Tied grabbing a middle mogo
  - Lift motor was not locked and went up, changed to hold motor when starting match
    - Tug of war test appears to work
- Detected pnuematic leak
  - Possibly in solenoid
- Robot didin't move
  - Program may not have been uploaded
- Controller likely causing issues

### 1/5/22
- Removed primary control disconnected warn
- Removed `inteldrive::runLPS`
  - Reasoning: not functional with precision.
- Implemented max velocity for `inteldrive::driveTo` and `inteldrive::turnTo`
- Added `inteldrive::driveInPolygon`
  - Reasoning: used to test PID constants
#### Testing
- Refined drive PID values

### 1/6/22
- Now using voltage control instead of percent for motors in `inteldrive`
  - Replaced with `vel * 150.0` using `mV` units
  - Reasoning: percent making weird calculations making robot more difficult to control
- Fixed memory permission error
  - Recaptured this in lambdas prior to usage


### 1/12/22
- Remove `TEST` macro
  - Reasoning: too cumbersome, easier to comment/uncomment
- Minor formatting tweaks
- Main thread contains prototype absolution location tracking
  - Will break autonomous with current version
- Updated `Readme.md`
  - Added `robot.png` depicting robot
  - Added information on code functionalities
  - Added robot configuration
  - Added team name
- Cleaned up some code
#### Testing
- Tested prototype absolute location tracking
  - Some innaccuracies while turning
    - Likely due to slipping
  - Going straight appears to track well

### 1/13/22
- Added `deltaTracker`
  - Reasoning: track anything that changes easier (e.g. time, motor revolutions)
- Removed `inteldrive::resetPosition` and `inteldrive::resetHeading`
  - Algorithms instead now uses `deltaTracker` or other equivalent method
  - Reasoning: force resetting may mess other algorithms
- Removed `inteldrive::driveTo(vec2...)` (drive to displacement)
  - Reasoning: unused
- Renamed parameter `additive` to `relative` in `inteldrive::driveTo` and `inteldrive::turnTo`
  - `relative` has opposite/inverted meaning `additive`
  - Reasoning: unclear meaning
#### Testing
- Using voltage to control motors causes inconsistent speeds
  - Want some method to guarentee straightness

### 1/19/22
- Added `trackLocation` function, `location` variable, and `getLocation` function
  - Moved location tracking here
  - Tracking is done in a different thread
#### Testing
- Tested location tracking
  - Fixed bugs
    - `rev2rad` needed for polar vector calculations
    - Use absolution rotation instead of change in rotation for displacement calculations
  - Location tracking appears to work in random testing
    - Displacement ± 2 inches
- Tested multithreaded location tracking
  - Variable appears not to replicate between threads

### 1/20/22
- Fixed date of `1/19/22` in `Log.md`
- Calculated drive to absolute location
  - Requires Testing
- Renamed `deltaTracker::dv` to `deltaTracker::delta`
  - Reasoning: better name
- Created `inteldrive::start`
  - Reasoning: able to initalize things outside of constructor 
    - Solves memory errors dealing with the change of `this` memory location for lambdas
- Removed `inteldrive::recapture`
  - Removed all callees
  - Reasoning: `inteldrive::start` takes functionality
- Removed empty `inteldrive` constructor
  - Reasoning: Any memory-prone issues now will get initalized in start
    - Now can use normal constructor globally and call `inteldrive::start` later
- Renamed `MEMBER_FUNCTION_THREAD` to `CREATE_METHOD_THREAD`
  - Reasoning: more descriptive name
- Updated `vec2` to have more cannon operators
- Started to implement `inteldrive::driveTo`
  - Uses displacement
#### Testing
- Location tracking thread now unable to access correct variables
  - Initialization of thread in constructor problematic

### 1/26/22
- Rewrote parts of PID
  - PID now templated for goal input
    - Defaulted to use `double`
    - Updated users to make change
    - No functionality now in `PID.cpp`
    - Debugging removed
      - May need to reimplement
    - Reasoning: can use `vec2` in input
  - PID now uses `deltaTracker`
    - Reasoning: cleans code up
- Removed `:`s in headers/subheaders in `Log.md`
  - Reasoning: consistency
#### Testing
- Tested PID
  - Appears to continue to work
- Tested `inteldrive::driveTo(vec2...)` 
  - Appears not to do anything

### 1/27/22
- Work lost
#### Testing
- Testing lost

### 2/2/22
- Added new parameter to `output` in `PID` of type `T` to transfer goal value
  - Updated users to align with new definition
  - Reasoning: displacement calculations require goal
- Worked on `inteldrive::driveTo(vec2...)`
  - Added some proper error calculation
- Updated `stevensonvexlogo.png` to new banner
- Made robot complete stop if no joystick input
- Made rainbow effect in its own thread instead of main thread
  - Reasoning: uicker changing rainbow
#### Testing
- Noticed robot drift if no input is pressed
  - Preferentially good/bad 
    - Momentum arguably intuitive however could be imprecise
- Tested `inteldrive::driveTo(vec2...)` 
  - Still does not do anything 
    - Tolerance appears to be too low

### 2/3/22
- Added `until` macro in `common.h`
- Constified parts of `common.h`
- Added timeout to `inteldrive::driveTo` and `inteldrive::driveTo`
  - Reasoning: respective PIDs may take too long to converge
- Changed `deltaTracker::operator++(int)` (postfix increment) to add delta to have total delta
  - Reasoning: total change from a point can be useful
- Tweaked turn PID values
- Removed `a -= std::floor(a)` from `angle_difference_rev`
- Timeout for `inteldrive::turnTo` now default to 500 ms
  - Reasoning: prevents from random stopping
#### Testing
- Simple straight auton functional
- Turning non-functional
  - Possibly due to lack of clean tires and tiles
  - Turning unoptimized direction
  - Retuned PID
- Robot decides to rotate towards incorrect direction
  - Likely issue with `angle_difference_rev`
- Timeout does not appear to work
- Spun around 1/4 rotation relatively
  - Turning hanged (timeout does not work)
  - Stopping error to large
    - Likely due to 'slippery' enviroment
- Controller sometimes disconnects
  - Unsure if this is normal

### 2/6/22 (Competition)
- Changed lift contorls to analog
  - Rumbles if at max
- Added new auton for WP
- Tuned pressure regulators
- Added `inteldrive::drive_straight`
  - Uses percent isntead of mV
  - Reasoning: drivePID auton expects straightness
- Slowed down `inteldrive::driveTo`
  - Done by lower PID values
- Made auton running on right side more comprenhensive
#### Competition
- Auton not clamping too late
  - Clamp later
  - Was not threaded
- Auton very inconsistent 
- Auton functions not doing expected things
  - Maybe running wrong programs

### 2/9/22
- Reimplemented `inteldrive::driveTo(vec2...)` back to turn and drive
  - Reasoning: other algorithm complex and untested
- Changed `inteldrive` to have fast variants of PID constants
  - Changed parameter footprint of `inteldrive::driveTo` and `inteldrive::turnTo`
#### Testing
- Timeout was not working
  - Bug in code (fixed)
- Tuned some PIDs
- Point and drive `inteldrive::driveTo` not functional
  - Tested absolute point (0, 0) (starting point)
    - Fail (appeared to somewhat keep straightness)
  - Tested relative point (0, 0) (same place)
    - Succeeded (does not move)
  - Possibly angle-location discrepancy 

### 2/10/22
- Fixed `2/9/22`'s log
  - Wrong date
  - Has colon after `Testing`
- Fixed `vex::ang`
  - Angle was being calculated incorrect
    - Used `tan` when `atan2` should have been used
#### Testing
- Point and drive `inteldrive::driveTo` not functional
  - Found error was with angle calculation
    - Angle was being miscalculated by `vec2`
- Point and drive now functional
  - Going 'home' (`{0, 0}`) absolute functional
  - Going 10in forward from home (`{10, 0}` absolute) functional
  - Going 10in side from home (`{0, 10}` absolute) functional
  - Going a distance from home (`{20, 10}` absolute) functional

### 2/16/22
- Made `inteldrive::driveTo(vec2...)` to default to absolute
  - Reasoning: generally used for absolute
- Changed `inteldrive::turnPID` to use `vex::voltageUnits::volt`
  - Retuned turn PID to match with new units
  - Reasoning: to get as much power out of the motor as possible

#### Testing
- Tuned turning PID
  - Did by turning .15 revolutions then .35 revolutions
  - Completely redid PID from scratch for volts
  - Accurate to `±.01` revolutions
  - kp = 105.0, ki = -32.0, kd = -25.0s

### 2/17/22
- Added back claw pnuematics
  - Toggles with back lift
- Flipped back pnuematic to invert method 
  - Reasoning: doesn't expell as much air

### 2/19/22 (Competition)
- Added new auton to use back claw (back claw rush)
#### Competition
- Back pnuematics leaking too much
  - Flip pnuematic tubes on the dual acting cylinder to lose less air
- Port 2 maybe burned out
  - Port 3 working

### 2/20/22
- Replaced `relative` bool with `reverse` parameter in `inteldrive::driveTo(vec2...)`
  - Reasoning: going backwards to specific location is required 
- Attempted self-correcting `driveTo`
  - Rewrote `inteldrive::driveTo(double...)` to use vectors
    - Used `PID<vec2>`
- Moved `kPID` outside `PID`
  - Reasoning: be consistent between templated versions of `PID`
- Reverted changes for self-correcting `driveTo`
#### Testing
- Backwards `driveTo` functional
- Self-correcting drive not functional
  - Jerks back and forth
  - Using vector instead of linear distance broken
- Self-correction may not be necessary with good wheels

### 2/21/22
- Rewrote code for new robot
  - Now one lift motor
    - Contains 3 states represented by an `enum`
  - Now two claws
  - Now intakes
- Updated controls
  - Button Y controls back claw
  - Lift analog function (`robot::liftAnalog`) removed
    - Controls functionally equivalent
    - Reasoning: to add 'hesitate' at center to lock there
- Added/updated some comments
- Updated ports
  - Updated `README.md`

### 2/24/22
- Separated `drivercontrol()` in `main.cpp` into several functions
  - Reasoning: cleans up function
- Added automatic intake controls
- Deleted limits on lift
- Deleted `PID.cpp`
  - Reasoning: not used
#### Testing
- Tested controls
  - Written controls working
    - Limits on lift not functional
- Tested drive PID
  - Refined speed version
- Tested pnuematic claws
- Tested lift

### 2/25/22 (Competition)
- Reverted Auton A to be forward
#### Testing
- Auton A appears to be consistents

## TODO
- Need to add controller config to `README.md`
  - Create diagrams 
- Refine PID values
- Finish reimplement `inteldrive::driveTo(vec2...)`
  - Maybe with curvature
  - Implement real-time autocorrecting `inteldrive::driveTo` 
- Re-add debugging for PID
  - Possibly multithreaded
- Test new controls