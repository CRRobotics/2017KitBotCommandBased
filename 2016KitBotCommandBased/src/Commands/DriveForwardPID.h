#ifndef DRIVE_FORWARD_PID_H
#define DRIVE_FORWARD_PID_H

#include "../CommandBase.h"
#include "../Robot.h"

class DriveForwardPID: public CommandBase {
public:
	DriveForwardPID(double driveDistance);
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
private:
	double driveDistance;
	double initialEncoderTicksR;
	double initialEncoderTicksL;
};

#endif  // EXAMPLE_COMMAND_H
