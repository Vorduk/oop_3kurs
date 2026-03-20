#pragma once
#include "BaseDevice.h"

// Увлажнитель воздуха
class AirHumidifier : public BaseDevice, public IModeSelectableDevice {
public:
    AirHumidifier();
    ~AirHumidifier() override = default;

    // Методы IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // Методы IModeSelectableDevice
    void setMode(int mode) override;
    int getMode() const override;

    static constexpr int MODE_LOWEST = 1;
    static constexpr int MODE_LOW = 2;
    static constexpr int MODE_MEDIUM = 3;
    static constexpr int MODE_HIGH = 4;

private:
    bool m_isOn;
    int m_mode; // 1 - слабый, 2 - средний, 3 - сильный

};
