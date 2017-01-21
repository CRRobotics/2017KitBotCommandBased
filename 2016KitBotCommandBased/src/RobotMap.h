#ifndef ROBOTMAP_H
#define ROBOTMAP_H

#include "SpeedController.h"
#include "WPILib.h"
#include "AHRS.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */

// For example to map the left and right motors, you could define the
// following variables to use with your drivetrain subsystem.
// constexpr int LEFTMOTOR = 1;
// constexpr int RIGHTMOTOR = 2;

class RobotMap {
public:
	static std::shared_ptr<VictorSP> driverDrive;
	static std::shared_ptr<VictorSP> drivelDrive;
	static std::shared_ptr<Encoder> driverEnc;
	static std::shared_ptr<Encoder> drivelEnc;
	static std::shared_ptr<AHRS> driveahrs;

	static void init();
// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
// constexpr int RANGE_FINDER_PORT = 1;
// constexpr int RANGE_FINDER_MODULE = 1;
};
#endif  // ROBOTMAP_H
