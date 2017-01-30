#include "AutoTurn.h"
#include "AngleMethods.h"
#define ENCODER_TICKS_PER_IN 483.74537
#define SLOW_ANGLE_BEGIN 45
#define SLOW_ANGLE_LIMIT 10

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
	double angleError = angle_diff(Robot::drive->GetYaw(), desiredAngle);
	printf("ANGLE_ERROR: %f\n", angleError);
	double direction = 1;
	if (angleError < 0)
	{
		direction = -1;
	}
	if(angleError > SLOW_ANGLE_BEGIN)
		Robot::drive->TankDrive(0.4 * direction, 0.4 * -direction, true);
	else if (angleError > SLOW_ANGLE_LIMIT)
		Robot::drive->TankDrive(fabs(angleError)/ SLOW_ANGLE_BEGIN * direction * 0.4, fabs(angleError)/ SLOW_ANGLE_BEGIN * -direction * 0.4, true);
	else
		Robot::drive->TankDrive(0.2 * direction, 0.2 * -direction, true);
}

// Make this return true when this Command no longer needs to run execute()
bool AutoTurn::IsFinished() {
	double angleError = angle_diff(Robot::drive->GetYaw(), desiredAngle);
	return fabs(angleError) < 5;
}

// Called once after isFinished returns true
void AutoTurn::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AutoTurn::Interrupted() {

}
