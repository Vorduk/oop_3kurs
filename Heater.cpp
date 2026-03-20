#include "Heater.h"

Heater::Heater()
    : BaseDevice("heater"), m_isOn(false), m_powerLevel(0) {
}

void Heater::turnOn() {
    m_isOn = true;
}

void Heater::turnOff() {
    m_isOn = false;
    m_powerLevel = 0;
}

bool Heater::isOn() const {
    return m_isOn;
}

void Heater::setPower(int level) {
    if (level < 0 || level > 100) {
        return;
    }
    if (m_isOn) {
        m_powerLevel = level;
    }
}

int Heater::getPower() const {
    return m_powerLevel;
}