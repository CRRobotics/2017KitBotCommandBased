#include "AutoTurn.h"
#include "AngleMethods.h"
#define ENCODER_TICKS_PER_IN 483.74537
#define SLOW_ANGLE_BEGIN 30
#define SLOW_ANGLE_LIMIT 8

AutoTurn::AutoTurn(double angle) {
	Requires(Robot::drive.get());
	desiredAngle = angle;
}

// Called just before this Command runs the first time
void AutoTurn::Initialize() {
	desiredAngle = frc::SmartDashboard::GetNumber("Drive Angle", 0);
}

// Called repeatedly when this Command is scheduled to run
void AutoTurn::Execute() {
	double angleError = angle_diff(desiredAngle, Robot::drive->GetYaw());
	double direction = 1;
	if (angleError < 0)
	{
		direction = -1;
	}
	if(angleError > SLOW_ANGLE_BEGIN)
		Robot::drive->TankDrive(0.4 * direction, 0.4 * -direction, true);
	else if (angleError > SLOW_ANGLE_LIMIT)
		Robot::drive->TankDrive(angleError/ SLOW_ANGLE_BEGIN * direction, angleError/ SLOW_ANGLE_BEGIN * -direction, true);
	else
		Robot::drive->TankDrive(0.2 * direction, 0.2 * -direction, true);
}

// Make this return true when this Command no longer needs to run execute()
bool AutoTurn::IsFinished() {
	double angleError = angle_diff(desiredAngle, Robot::drive->GetYaw());
	return fabs(angleError) < 4;
}

// Called once after isFinished returns true
void AutoTurn::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AutoTurn::Interrupted() {

}
