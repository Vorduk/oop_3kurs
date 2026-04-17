#include "Heater.h"

Heater::Heater(std::shared_ptr<IDeviceDriver> driver)
    : BaseDevice("heater"), m_driver(driver) {
}

void Heater::turnOn() {
    m_driver->hardwarePowerOn();
}

void Heater::turnOff() {
    m_driver->hardwarePowerOff();
}

bool Heater::isOn() const {
    return m_driver->isHardwareOn();
}

void Heater::setPower(int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    m_last_power = level;
    m_driver->hardwareSetPower(level);
}

int Heater::getPower() const {
    return m_last_power;
}