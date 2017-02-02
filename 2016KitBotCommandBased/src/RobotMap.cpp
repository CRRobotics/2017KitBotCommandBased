// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
std::shared_ptr<CANTalon> RobotMap::driverDrive;
std::shared_ptr<CANTalon> RobotMap::drivelDrive;
std::shared_ptr<Encoder> RobotMap::driverEnc;
std::shared_ptr<Encoder> RobotMap::drivelEnc;
std::shared_ptr<AHRS> RobotMap::driveahrs;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION

void RobotMap::init() {
    LiveWindow *lw = frc::LiveWindow::GetInstance();

    driverDrive.reset(new CANTalon(0));
    lw->AddActuator("Drive", "rDrive", std::static_pointer_cast<CANTalon>(driverDrive));
    
    drivelDrive.reset(new CANTalon(1));
    lw->AddActuator("Drive", "lDrive", std::static_pointer_cast<CANTalon>(drivelDrive));
    
    driveahrs.reset(new AHRS(frc::SPI::kMXP));
//     driveahrs.reset(new AHRS(frc::SerialPort::kUSB));

    driverEnc.reset(new Encoder(2,3, false, Encoder::EncodingType::k4X));
    drivelEnc.reset(new Encoder(0,1, true, Encoder::EncodingType::k4X));
    driverEnc->Reset();
    driverEnc->Reset();
    driverEnc->SetDistancePerPulse(6 * 3.141592653589 /1024.);
    drivelEnc->SetDistancePerPulse(6 * 3.141592653589 /1024.);
}
