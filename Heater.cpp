#include "Heater.h"

Heater::Heater()
    : BaseDevice("heater"), m_is_on(false), m_powerLevel(0) {
}

void Heater::turnOn() {
    m_is_on = true;
}

void Heater::turnOff() {
    m_is_on = false;
    m_powerLevel = 0;
}

bool Heater::isOn() const {
    return m_is_on;
}

void Heater::setPower(int level) {
    if (level < 0 || level > 100) {
        return;
    }
    if (m_is_on) {
        m_powerLevel = level;
    }
}

int Heater::getPower() const {
    return m_powerLevel;
}