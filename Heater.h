#pragma once

#include "IDevice.h"

class Heater : public IDevice, public IAdjustableDevice {
private:
    bool m_isOn;
    int m_powerLevel;  // 0-100%

public:
    Heater();
    ~Heater() override = default;

    // Методы IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // Методы IAdjustableDevice
    void setPower(int level) override;
    int getPower() const override;
};

