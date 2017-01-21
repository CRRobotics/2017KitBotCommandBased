// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "FollowTheBox.h"
#include "../Subsystems/Drive.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

FollowTheBox::FollowTheBox(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::drive.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void FollowTheBox::Initialize() {
	NetworkTable::SetServerMode();
	NetworkTable::SetIPAddress("0.0.0.0");
//	NetworkTable::SetTeam(639);
	table = NetworkTable::GetTable("CameraTracker");
	table->PutNumber("XAngleToTarget", 0.0);
	table->PutNumber("TargetX", 0.0);
	table->PutNumber("TargetY", 0.0);
}

// Called repeatedly when this Command is scheduled to run
void FollowTheBox::Execute() {
	double angle = table->GetNumber("XAngleToTarget", 0.0);
	double targetX = table->GetNumber("TargetX", 0.0);
	double targetY = table->GetNumber("TargetY", 0.0);
	SmartDashboard::PutNumber("Angle", angle);
	SmartDashboard::PutNumber("TargetX", targetX);
	SmartDashboard::PutNumber("TargetY", targetY);
	printf("being run; number is %f\n", angle);
	double rSpeed = 0;
	double lSpeed = 0;
	if (fabs(angle) > 2)
	{
		if (fabs(angle) > 20)
		{
			rSpeed = 0.25;

		}
		else if (fabs(angle) > 10)
		{
			rSpeed = (fabs(angle) / 20) * 0.25;
		}
		else
		{
			rSpeed = 0.08;
		}
			lSpeed = rSpeed;
	}
	if (angle < 0)
	{
		rSpeed = rSpeed * -1;
	}
	else
	{
		lSpeed = lSpeed * -1;
	}
	Robot::drive->TankDrive(lSpeed, rSpeed, false);
}

// Make this return true when this Command no longer needs to run execute()
bool FollowTheBox::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void FollowTheBox::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FollowTheBox::Interrupted() {

}
