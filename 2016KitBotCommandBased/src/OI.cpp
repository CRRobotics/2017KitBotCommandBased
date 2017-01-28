#include "OI.h"

#include <WPILib.h>
#include "Joystick.h"

OI::OI() {
	rJoystick.reset(new frc::Joystick(1));
	lJoystick.reset(new frc::Joystick(0));
}

double OI::GetLJoystick(){
	if (fabs(lJoystick->GetY()) > 0.1)
		return lJoystick->GetY();
	return 0;
}

double OI::GetRJoystick(){
	if (fabs(rJoystick->GetY()) > 0.1)
		return rJoystick->GetY();
	return 0;
}


