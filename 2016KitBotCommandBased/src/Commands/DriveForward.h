#ifndef DRIVE_FORWARD_H
#define DRIVE_FORWARD_H

#include "../CommandBase.h"
#include "../Robot.h"

class DriveForward: public CommandBase {
public:
	DriveForward(double driveDistance);
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
private:
	double driveDistance;
	double initialEncoderTicks;
};

#endif  // EXAMPLE_COMMAND_H
