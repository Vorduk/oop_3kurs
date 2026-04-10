#include "DeviceDecorator.h"

DeviceDecorator::DeviceDecorator(std::shared_ptr<IDevice> device)
    : m_device(device) {
}

// IDevice
void DeviceDecorator::turnOn() {
    m_device->turnOn();
}

void DeviceDecorator::turnOff() {
    m_device->turnOff();
}

bool DeviceDecorator::isOn() const {
    return m_device->isOn();
}

std::string DeviceDecorator::getType() const {
    return m_device->getType();
}

int DeviceDecorator::getId() const {
    return m_device->getId();
}

// IAdjustableDevice
void DeviceDecorator::setPower(int level) {
    std::shared_ptr<IAdjustableDevice> adjustable_device = std::dynamic_pointer_cast<IAdjustableDevice>(m_device);
    if (adjustable_device) {
        adjustable_device->setPower(level);
    }
}

int DeviceDecorator::getPower() const {
    std::shared_ptr<IAdjustableDevice> adjustable_device = std::dynamic_pointer_cast<IAdjustableDevice>(m_device);
    if (adjustable_device) {
        return adjustable_device->getPower();
    }
    return 0;
}

// IModeSelectableDevice
void DeviceDecorator::setMode(int mode) {
    std::shared_ptr<IModeSelectableDevice> mode_selectable_device = std::dynamic_pointer_cast<IModeSelectableDevice>(m_device);
    if (mode_selectable_device) {
        mode_selectable_device->setMode(mode);
    }
}

int DeviceDecorator::getMode() const {
    std::shared_ptr<IModeSelectableDevice> mode_selectable_device = std::dynamic_pointer_cast<IModeSelectableDevice>(m_device);
    if (mode_selectable_device) {
        return mode_selectable_device->getMode();
    }
    return 0;
}