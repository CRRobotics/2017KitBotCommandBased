#include "GearAlignment.h"
#include "FollowTheBoxBetter.h"
#include "AngleMethods.h"

std::shared_ptr<NetworkTable> GearAlignment::table;


GearAlignment::GearAlignment() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void GearAlignment::Initialize() {
	table = FollowTheBoxBetter::table;
	if (table == nullptr)
	{
		NetworkTable::SetServerMode();
		NetworkTable::SetIPAddress("0.0.0.0");
	//	NetworkTable::SetTeam(639);
		table = NetworkTable::GetTable("CameraTracker");
	}
	absoluteAngleOfTarget = Robot::drive->GetYaw();
	lastTick = table->GetNumber("ticker", 0);
}

// Called repeatedly when this Command is scheduled to run
void GearAlignment::Execute()
{
	double currentTick = table->GetNumber("ticker", lastTick);
	if (currentTick != lastTick)
	{
		double angleToTarget = GearAlignment::table->GetNumber("XAngleToTarget", 0.0);
		absoluteAngleOfTarget = formatAngle(Robot::drive->GetYaw() + angleToTarget);
		lastTick = currentTick;
	}
	double angleDiff = angle_diff(Robot::drive->GetYaw(), absoluteAngleOfTarget);
	if (abs(angleDiff) < 2)
	{
		Robot::drive->TankDrive(0.3, 0.3, true);
	}
	else if (angleDiff > 0)
	{

	}
	else
	{

	}
}

// Make this return true when this Command no longer needs to run execute()
bool GearAlignment::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void GearAlignment::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void GearAlignment::Interrupted() {

}
