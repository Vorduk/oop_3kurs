#pragma once
#include "DeviceDecorator.h"
#include <iostream>

class DevicePowerLimitDecorator : public DeviceDecorator {
private:
    int m_max_power;  // максимально допустимая мощность (0-100)

public:
    DevicePowerLimitDecorator(std::shared_ptr<IDevice> device, int maxPower);

    void setPower(int level) override;
};

