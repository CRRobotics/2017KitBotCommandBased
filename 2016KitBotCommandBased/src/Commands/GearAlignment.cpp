#include "GearAlignment.h"

GearAlignment::GearAlignment() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void GearAlignment::Initialize() {
	/*if (table == nullptr)
		{
			NetworkTable::SetServerMode();
			NetworkTable::SetIPAddress("0.0.0.0");
		//	NetworkTable::SetTeam(639);
			table = NetworkTable::GetTable("CameraTracker");
		}*/


}

// Called repeatedly when this Command is scheduled to run
void GearAlignment::Execute() {


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
