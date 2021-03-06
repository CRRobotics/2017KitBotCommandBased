 // RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVE_H
#define DRIVE_H
#include "Commands/Subsystem.h"
#include "RobotMap.h"
#include "../AngleMethods.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Drive: public Subsystem {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<CANTalon> lDrive;
//	std::shared_ptr<Encoder> rEnc;
//	std::shared_ptr<Encoder> lEnc;
	std::shared_ptr<AHRS> ahrs;

	/*double rateRP;
	double rateRI;
	double rateRD;
	double rateLP;
	double rateLI;
	double rateLD;
	double currentSpeedR;
	double currentSpeedL;*/
	//std::shared_ptr<PIDController> lSpeedController;
	//std::shared_ptr<PIDController> rSpeedController;

	void SpeedControlDrive(double l, double r);
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
public:
	Drive();
	void InitDefaultCommand();
	void TankDrive(double l, double r, bool speedControl = false);
	void SetPID(double p, double i, double d, double f);
	void SetControlMode(CANTalon::ControlMode mode);
	double GetYaw();
	int GetREnc();
	int GetLEnc();
	int GetREncRate();
	int GetLEncRate();
	void DriveToTick(int tick);
	void ResetTalonIntegral();
	double speedControlConstantP = 0.05;
	double speedControlConstantI = 0;
	double speedControlConstantD = 0;
	double speedControlConstantF = 0;
	double positionControlP = 0.05;
	double positionControlI = 0;
	double positionControlD = 0;
	std::shared_ptr<CANTalon> rDrive;//TODO make private

	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
};

#endif
