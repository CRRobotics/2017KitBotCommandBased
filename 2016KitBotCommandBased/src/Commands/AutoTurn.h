#ifndef AUTO_TURN_H
#define AUTO_TURN_H

#include "../CommandBase.h"
#include "../Robot.h"

class AutoTurn: public CommandBase {
public:
	AutoTurn(double driveDistance);
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
private:
	double desiredAngle;
	double initialAngle;
};

#endif  // EXAMPLE_COMMAND_H
