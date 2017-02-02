#include "DriveForwardPID.h"
#define ENCODER_TICKS_PER_IN 51.1444
#define SLOW_BEGIN 2000

DriveForwardPID::DriveForwardPID(double dist) {
	Requires(Robot::drive.get());
	driveDistance = dist;
}

// Called just before this Command runs the first time
void DriveForwardPID::Initialize() {
	driveDistance = frc::SmartDashboard::GetNumber("Drive Distance", 0);
	initialEncoderTicksR = RobotMap::driverEnc->Get();
	initialEncoderTicksL = RobotMap::drivelEnc->Get();
	Robot::drive->positionControlP = SmartDashboard::GetNumber("Pos_P", 0);
	Robot::drive->positionControlI = SmartDashboard::GetNumber("Pos_I", 0);
	Robot::drive->positionControlD = SmartDashboard::GetNumber("Pos_D", 0);
	Robot::drive->DriveToTick(driveDistance * ENCODER_TICKS_PER_IN);
}

// Called repeatedly when this Command is scheduled to run
void DriveForwardPID::Execute() {

}

// Make this return true when this Command no longer needs to run execute()
bool DriveForwardPID::IsFinished() {
	double placementErrorR = initialEncoderTicksR +
				driveDistance * ENCODER_TICKS_PER_IN -  RobotMap::driverEnc->Get();
	double placementErrorL = initialEncoderTicksL +
					driveDistance * ENCODER_TICKS_PER_IN -  RobotMap::drivelEnc->Get();
	return fabs(placementErrorR) < 30 && fabs(placementErrorL) < 30;
}

// Called once after isFinished returns true
void DriveForwardPID::End() {
	Robot::drive->TankDrive(0, 0, true);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveForwardPID::Interrupted() {

}
