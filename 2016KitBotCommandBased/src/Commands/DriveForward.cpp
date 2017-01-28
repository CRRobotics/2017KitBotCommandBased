#include "DriveForward.h"
#define ENCODER_TICKS_PER_IN 483.74537
#define SLOW_BEGIN 5000

DriveForward::DriveForward(double dist) {
	Requires(Robot::drive.get());
	driveDistance = dist;
}

// Called just before this Command runs the first time
void DriveForward::Initialize() {
	driveDistance = frc::SmartDashboard::GetNumber("Drive Distance", 0);
	initialEncoderTicks = RobotMap::driverEnc->Get();
}

// Called repeatedly when this Command is scheduled to run
void DriveForward::Execute() {
	double placementError = initialEncoderTicks +
			driveDistance * ENCODER_TICKS_PER_IN - RobotMap::driverEnc->Get();
	if(placementError > SLOW_BEGIN)
		Robot::drive->TankDrive(0.4, 0.4, true);
	else if (placementError > SLOW_BEGIN * 0.2 )
		Robot::drive->TankDrive(placementError / SLOW_BEGIN, placementError / SLOW_BEGIN, true);
	else
		Robot::drive->TankDrive(0.2, 0.2, true);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveForward::IsFinished() {
	double placementError = initialEncoderTicks +
				driveDistance / ENCODER_TICKS_PER_IN -  RobotMap::driverEnc->Get();
	return fabs(placementError) < 100;
}

// Called once after isFinished returns true
void DriveForward::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveForward::Interrupted() {

}
