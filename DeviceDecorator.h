#pragma once
#include "IDevice.h"
#include <memory>

class DeviceDecorator : public IDevice, public IAdjustableDevice, public IModeSelectableDevice {
protected:
    std::shared_ptr<IDevice> m_device;

public:
    DeviceDecorator(std::shared_ptr<IDevice> device) : m_device(device) {}

    // IDevice
    void turnOn() override { m_device->turnOn(); }
    void turnOff() override { m_device->turnOff(); }
    bool isOn() const override { return m_device->isOn(); }
    std::string getType() const override { return m_device->getType(); }
    int getId() const override { return m_device->getId(); }

    // IAdjustableDevice
    void setPower(int level) override {
        if (auto adj = std::dynamic_pointer_cast<IAdjustableDevice>(m_device)) {
            adj->setPower(level);
        }
    }
    int getPower() const override {
        if (auto adj = std::dynamic_pointer_cast<IAdjustableDevice>(m_device)) {
            return adj->getPower();
        }
        return 0;
    }

    // IModeSelectableDevice
    void setMode(int mode) override {
        if (auto modeSel = std::dynamic_pointer_cast<IModeSelectableDevice>(m_device)) {
            modeSel->setMode(mode);
        }
    }
    int getMode() const override {
        if (auto modeSel = std::dynamic_pointer_cast<IModeSelectableDevice>(m_device)) {
            return modeSel->getMode();
        }
        return 0;
    }
};