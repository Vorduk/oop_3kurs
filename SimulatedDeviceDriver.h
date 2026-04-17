#pragma once
#include "IDeviceDriver.h"

class SimulatedDeviceDriver : public IDeviceDriver {
public:
    void hardwarePowerOn() override;
    void hardwarePowerOff() override;
    void hardwareSetPower(int level) override;
    bool isHardwareOn() const override;

private:
    bool m_isOn = false;
    int m_power = 0;
};
