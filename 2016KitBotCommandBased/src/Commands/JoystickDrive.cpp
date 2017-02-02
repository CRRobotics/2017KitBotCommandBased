// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "JoystickDrive.h"
#include "../Subsystems/Drive.h"
#include "FollowTheBoxBetter.h"
#define MAX_ENC_SPEED 150

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR
std::shared_ptr<NetworkTable> table;

JoystickDrive::JoystickDrive(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::drive.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void JoystickDrive::Initialize() {
	if (FollowTheBoxBetter::table == nullptr)
	{
		NetworkTable::SetServerMode();
		NetworkTable::SetIPAddress("0.0.0.0");
	//	NetworkTable::SetTeam(639);
		table = NetworkTable::GetTable("CameraTracker");
		table->PutNumber("XAngleToTarget", 0.0);
		table->PutNumber("TargetX", 0.0);
		table->PutNumber("TargetY", 0.0);
		table->PutNumber("ticker", 0.0);
	}
}

// Called repeatedly when this Command is scheduled to run
void JoystickDrive::Execute() {
	Robot::drive->TankDrive(Robot::oi->GetLJoystick() * fabs(Robot::oi->GetLJoystick()), Robot::oi->GetRJoystick() * fabs(Robot::oi->GetRJoystick()));
	//Robot::drive->TankDrive(0, 0);
//	frc::SmartDashboard::PutNumber("rEnc", RobotMap::driverEnc->GetRate());
//	frc::SmartDashboard::PutNumber("lEnc", RobotMap::drivelEnc->GetRate());
	Robot::drive->SetPID(frc::SmartDashboard::GetNumber("Speed_P", 0),
						 frc::SmartDashboard::GetNumber("Speed_I", 0),
						 frc::SmartDashboard::GetNumber("Speed_D", 0),
						 frc::SmartDashboard::GetNumber("Speed_F", 0));
	double targetY = table->GetNumber("TargetY", 0.0);
	double angleY = (480 - targetY) * 0.078445;
	angleY = (15.1397 - 18.8268) + angleY;
	double theoreticalDistance = (88 - 5 + 2) / tan(angleY * M_PI / 180);
	frc::SmartDashboard::PutNumber("AngleY", angleY);
	frc::SmartDashboard::PutNumber("DistanceToTarget", theoreticalDistance);
}

// Make this return true when this Command no longer needs to run execute()
bool JoystickDrive::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void JoystickDrive::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void JoystickDrive::Interrupted() {

}
