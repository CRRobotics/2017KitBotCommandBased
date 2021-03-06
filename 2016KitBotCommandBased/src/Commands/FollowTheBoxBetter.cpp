// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "FollowTheBoxBetter.h"
#include "../Subsystems/Drive.h"

double desiredAngle;
double lastTick;
std::shared_ptr<NetworkTable> FollowTheBoxBetter::table;

FollowTheBoxBetter::FollowTheBoxBetter(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::drive.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void FollowTheBoxBetter::Initialize() {
	if (table == nullptr)
	{
		NetworkTable::SetServerMode();
		NetworkTable::SetIPAddress("0.0.0.0");
	//	NetworkTable::SetTeam(639);
		table = NetworkTable::GetTable("CameraTracker");
		table->PutNumber("XAngleToTarget", 0.0);
		table->PutNumber("TargetX", 0.0);
		table->PutNumber("TargetY", 0.0);
		table->PutNumber("ticker", 0.0);
		lastTick = 0;
	}
	desiredAngle = Robot::drive->GetYaw();
}

// Called repeatedly when this Command is scheduled to run
void FollowTheBoxBetter::Execute() {
	double currentTick = table->GetNumber("ticker", 0.0);
	double currentAngle = Robot::drive->GetYaw();
	double newAngleRelative = table->GetNumber("XAngleToTarget", 0.0);
	if (currentTick != lastTick)
	{
		lastTick = currentTick;
		desiredAngle = formatAngle(currentAngle + newAngleRelative);
	}
	double angleDiff = angle_diff(desiredAngle, currentAngle);
	SmartDashboard::PutNumber("AngleFromPI", newAngleRelative);
//	SmartDashboard::PutNumber("CurrentYaw", currentAngle);
	SmartDashboard::PutNumber("AngleDiffCalculated", angleDiff);
	SmartDashboard::PutNumber("Tick", currentTick);
	SmartDashboard::PutNumber("TargetX", table->GetNumber("TargetX", 0.0));
	SmartDashboard::PutNumber("TargetY", table->GetNumber("TargetY", 0.0));
	printf("being run; number is %f\n", desiredAngle);
	double rSpeed = 0;
	double lSpeed = 0;
	if (fabs(angleDiff) > 2)
	{
		if (fabs(angleDiff) > 30)
		{
			rSpeed = 0.4;
		}
		else if (fabs(angleDiff) > 10)
		{
			rSpeed = (fabs(angleDiff) / 30) * 0.4;
		}
		else
		{
				rSpeed = 0.15;
		}
		lSpeed = rSpeed;
	}
	if (angleDiff > 0)
	{
		rSpeed = rSpeed * -1;
	}
	else
	{
		lSpeed = lSpeed * -1;
	}

	Robot::drive->TankDrive(lSpeed, rSpeed, true);
}

// Make this return true when this Command no longer needs to run execute()
bool FollowTheBoxBetter::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void FollowTheBoxBetter::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FollowTheBoxBetter::Interrupted() {

}
