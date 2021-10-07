# 2021 - 2022 Vex 1965Y Coding Log

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
- Reordered paramters in `inteldrive` constructor
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