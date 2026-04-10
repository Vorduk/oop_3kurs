#pragma once
#include "DeviceDecorator.h"
#include <iostream>
#include <ctime>

class DeviceLoggerDecorator : public DeviceDecorator {
private:

public:
    DeviceLoggerDecorator(std::shared_ptr<IDevice> device);

    void turnOn() override;
    void turnOff() override;
    void setPower(int level) override;
    void setMode(int mode) override;
};

