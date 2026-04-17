#pragma once
#include "IDevice.h"
#include <memory>

class DeviceDecorator : public IDevice, public IAdjustableDevice, public IModeSelectableDevice {
protected:
    std::shared_ptr<IDevice> m_device;

public:
    DeviceDecorator(std::shared_ptr<IDevice> device);

    // IDevice
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;
    std::string getType() const override;
    int getId() const override;

    // IAdjustableDevice
    void setPower(int level) override;
    int getPower() const override;

    // IModeSelectableDevice
    void setMode(int mode) override;
    int getMode() const override;
};