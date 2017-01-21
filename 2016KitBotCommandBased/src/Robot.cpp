#include <memory>
#include "Robot.h"
#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#include "Commands/ExampleCommand.h"
#include "CommandBase.h"
#include "Commands/PushBackDrive.h"
#include "Commands/FollowTheBox.h"
#include "Commands/FollowTheBoxBetter.h"

std::unique_ptr<OI> Robot::oi;
std::shared_ptr<Drive> Robot::drive;
std::unique_ptr<frc::Command> autonomousCommand;
frc::SendableChooser<frc::Command*> chooser;


	void Robot::RobotInit() {
		RobotMap::init();
		drive.reset(new Drive());
		oi.reset(new OI());
		//chooser.AddDefault("Default Auto", new ExampleCommand());
		// chooser.AddObject("My Auto", new MyAutoCommand());
		//frc::SmartDashboard::PutData("Auto Modes", &chooser);
		frc::SmartDashboard::PutNumber("PCons", -0.005);
		frc::SmartDashboard::PutNumber("DCons", 0.005);
		frc::SmartDashboard::PutData("PushBackDrive", new PushBackDrive());
		frc::SmartDashboard::PutData("Follow Target", new FollowTheBox());
		frc::SmartDashboard::PutData("Follow Target with NavX", new FollowTheBoxBetter());
		frc::SmartDashboard::PutNumber("Speed_P ", -.005);
		frc::SmartDashboard::PutNumber("Speed_I", 0.0);
		frc::SmartDashboard::PutNumber("Speed_D", 0.05);
	}

	/**
	 * This function is called once each time the robot enters Disabled mode.
	 * You can use it to reset any subsystem information you want to clear when
	 * the robot is disabled.
	 */
	void Robot::DisabledInit() {

	}

	void Robot::DisabledPeriodic() {
		frc::Scheduler::GetInstance()->Run();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString code to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional commands to the
	 * chooser code above (like the commented example) or additional comparisons
	 * to the if-else structure below with additional strings & commands.
	 */
	void Robot::AutonomousInit() {
		/* std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
		if (autoSelected == "My Auto") {
			autonomousCommand.reset(new MyAutoCommand());
		}
		else {
			autonomousCommand.reset(new ExampleCommand());
		} */

		autonomousCommand.reset(chooser.GetSelected());

		if (autonomousCommand.get() != nullptr) {
			autonomousCommand->Start();
		}
	}

	void Robot::AutonomousPeriodic(){
		frc::Scheduler::GetInstance()->Run();
	}

	void Robot::TeleopInit()  {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to
		// continue until interrupted by another command, remove
		// this line or comment it out.
		if (autonomousCommand != nullptr) {
			autonomousCommand->Cancel();
		}
	}

	void Robot::TeleopPeriodic() {
		frc::Scheduler::GetInstance()->Run();
		if (!RobotMap::driveahrs->IsCalibrating())
			frc::SmartDashboard::PutNumber("CurrentYaw", Robot::drive->GetYaw());
	    frc::SmartDashboard::PutNumber("rEnc",RobotMap::driverEnc->Get());
	    frc::SmartDashboard::PutNumber("lEnc",RobotMap::drivelEnc->Get());
	    frc::SmartDashboard::PutNumber("LJoystick", Robot::oi->GetLJoystick());
	    frc::SmartDashboard::PutNumber("RJoystick", Robot::oi->GetRJoystick());

	    //printf("%d %d\n", RobotMap::driverEnc->Get(), RobotMap::drivelEnc->Get());
	}

	void Robot::TestPeriodic() {
		frc::LiveWindow::GetInstance()->Run();
	}



START_ROBOT_CLASS(Robot)
