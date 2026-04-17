#include "Conditioner.h"

Conditioner::Conditioner(std::shared_ptr<IDeviceDriver> driver)
    : BaseDevice("conditioner"), m_driver(driver) {
}

void Conditioner::turnOn() {
    m_driver->hardwarePowerOn();
}

void Conditioner::turnOff() {
    m_driver->hardwarePowerOff();
}

bool Conditioner::isOn() const {
    return m_driver->isHardwareOn();
}

void Conditioner::setPower(int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    m_last_power = level;
    m_driver->hardwareSetPower(level);
}

int Conditioner::getPower() const {
    return m_last_power;
}