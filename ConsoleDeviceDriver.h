// ConsoleDeviceDriver.h
#pragma once
#include "IDeviceDriver.h"
#include <iostream>
#include <string>

class ConsoleDeviceDriver : public IDeviceDriver {
public:
    ConsoleDeviceDriver(const std::string& name);
    void hardwarePowerOn() override;
    void hardwarePowerOff() override;
    void hardwareSetPower(int level) override;
    bool isHardwareOn() const override;

private:
    std::string m_device_name;
    bool m_isOn = false;
    int m_power = 0;
};