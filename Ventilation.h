#pragma once
#include "BaseDevice.h"

class Ventilation : public BaseDevice, public IModeSelectableDevice {
private:
    bool m_isOn;
    int m_mode;  // 1 - слабый, 2 - средний, 3 - сильный

public:
    Ventilation();
    ~Ventilation() override = default;

    // Методы IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // Методы IModeSelectableDevice
    void setMode(int mode) override;
    int getMode() const override;

    // Дополнительные константы для режимов
    static constexpr int MODE_LOW = 1;
    static constexpr int MODE_MEDIUM = 2;
    static constexpr int MODE_HIGH = 3;
};

