#pragma once
#include "BaseDevice.h"

// Ирригация
class Irrigation : public BaseDevice, public IModeSelectableDevice {
private:
    bool m_isOn;
    int m_mode;  // 1 - капельный полив, 2 - разбрызгивание

public:
    Irrigation();
    ~Irrigation() override = default;

    //  Методы IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // Методы IModeSelectableDevice
    void setMode(int mode) override;
    int getMode() const override;

    static constexpr int MODE_DRIP = 1;      // Капельный полив
    static constexpr int MODE_SPRINKLER = 2;  // Разбрызгивание
};