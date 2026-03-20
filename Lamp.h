#pragma once
#include "BaseDevice.h"

class Lamp : public BaseDevice, public IAdjustableDevice, public IModeSelectableDevice {
private:
    bool m_isOn;
    int m_powerLevel;  // 0-100% яркость
    int m_mode;        // 1 - белый, 2 - теплый, 3 - ультрафиолет

public:
    Lamp();
    ~Lamp() override = default;

    // IDevice implementation
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    // IAdjustableDevice implementation
    void setPower(int level) override;
    int getPower() const override;

    // IModeSelectableDevice implementation
    void setMode(int mode) override;
    int getMode() const override;

    static constexpr int MODE_WHITE = 1;
    static constexpr int MODE_WARM = 2;
    static constexpr int MODE_UF = 3;
};

