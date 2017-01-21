#ifndef OI_H
#define OI_H
#include "Joystick.h"
class OI {
public:
	double GetLJoystick();
	double GetRJoystick();
	OI();
private:
	std::shared_ptr<Joystick> lJoystick;
	std::shared_ptr<Joystick> rJoystick;
};

#endif  // OI_H
