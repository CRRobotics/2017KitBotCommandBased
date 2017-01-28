#ifndef GEAR_ALIGNMENT_H
#define GEAR_ALIGNMENT_H

#include "../CommandBase.h"
#include "../Robot.h"

class GearAlignment: public CommandBase {
public:
	GearAlignment();
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
	static	std::shared_ptr<NetworkTable> table;
private:

};

#endif  // EXAMPLE_COMMAND_H
