#pragma once
#include "BaseDevice.h"

// Кондиционер 
class Conditioner : public BaseDevice, public IAdjustableDevice {
private:
    bool m_isOn;
    int m_powerLevel;  // 0-100% (интенсивность охлаждения)

public:
    Conditioner();
    ~Conditioner() override = default;

    // Методы IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // Методы IAdjustableDevice
    void setPower(int level) override;
    int getPower() const override;
};

