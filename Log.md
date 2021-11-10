# 2021  - 2022 Vex 1965Y Coding Log

### 09/09/21:
- Copied `PID.h` and `PID.cpp` from [IyadHamid/StevensonVex1965Y-ChangeUp](https://github.com/IyadHamid/StevensonVex1965Y-ChangeUp)
- Edited `PID.h` and `PID.cpp`
  - Added additional comments/descriptions
  - Accounted for time (`dt`) in equations

### 09/15/21:
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

### 09/22/2021:
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

### 09/23/2021:
- Reordered parameters in `inteldrive` constructor
- Made `Log.txt` to `Log.md`
  - Reformatted Log to markdown
- Added definitions to `robot.cpp`
  - Added `constexpr` variables for constants such as ports and robot 
- Added some functionality to `main.cpp`
  - Added `vex::competion` instance
  - Added `drivercontrol` function
  - Added `autonomous` function
 
### 09/29/2021:
- Tested pnuematics control (through `vex::pneumatics`) and confirmed that it works

### 09/30/2021:
- Removed `dt` from `PID::run` as error is already relative to time
- Commented `inteldrive::arcTo` function out to build/test curve
- Added testing code to run `inteldrive::driveTo`
- Added comment descriptors for variables in `inteldrive.h`
- Added support for `inchesRatio` in `inteldrive`
  - Now is ratio for inches : spin
  - `constexpr wheel2inches_ratio` now `constexpr inches2wheel_ratio` in `robot.h` to match
#### Testing:
- Made an attempt to run `inteldrive::driveTo`
  - Does not work, nothing happens
#### Notes:
- Need to debug and fix `inteldrive::driveTo`

### 10/04/2021:
- Added missing `Log.md` to github repository
- Updated `README.md` to mention `Log.md`

### 10/06/2021:
- Fixed link formating in `Log.md`
- Added debugging to `PID` enabled by `DEBUG` macro in `PID.h`
- Added new log category to `Log.md` titled `Testing`specifically to `09/30/2021`
#### Testing:
- Tested `inteldrive::driveTo`
  - Does not work, found program hangs in `PID`

### 10/07/21
- Made debugging for `PID::run` viewable/function
- Replaced `not` with `!` in `inteldrive.cpp`
 #### Testing:
- Isolated problem from above to `vex::motor` constructor
  - Fixed by using `vex::PORT{number}` instead of literals.
  - Found another issue with `PID`/`inteldrive` not updating position error

### 10/13/21
#### Testing:
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
#### Testing: 
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
#### Testing:
- Experimented with accelerometer of inertial sensor
  - Accelerometer is not accurate/local enough to be useful

### 11/3/21
- Removed `vex::directionType` parameter from `inteldrive::drive`
  - Replicated to function callees
  - Reasoning: direction is implied from sign of `double vel` parameter, cleans up code
#### Notes:
- Github does not format `Log.md` correctly
  - Likely due to space vs tab
#### Testing:
- Tested physical base
  - Getting memory access error when using `idrive` from `robot.cpp`
    - Duct-tape fix of creating new `inteldrive` and using that works
    - Unable to fix

### 1/4/21
- Renamed `lmiddle` and `rmiddle` to `ltop` and `rtop` respectively
  - Reasoning: matches what they are called
- Moved `inertial` constructor outside of `inteldrive` constructor in `robot.cpp`
  - Reasoning: fixes possible memory access error like `left` and `right` `motor_group`s
- Added `claw` variable of `vex::pnuematics` in `robot.h`, `robot.cpp`
  - Initialized with `CLAW_PORT` in `config.h`
- Replaced single spaces with double spaces in `Log.md` to fix list formating in Github
- Updated `inches2wheel_ratio` in `config.h`
#### Testing:
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

## TODO
- Possibly use `std::max` or multiplication to set speed in `PID`
- Need to add things to `README.md`
  - Add controls and wiring
    - Create diagrams for each
- Need to fix units for `drivePID` and `turnPID` velocities