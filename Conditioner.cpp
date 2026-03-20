#include "Conditioner.h"

Conditioner::Conditioner()
    : BaseDevice("conditioner"), m_isOn(false), m_powerLevel(0) {
}

void Conditioner::turnOn() {
    m_isOn = true;
}

void Conditioner::turnOff() {
    m_isOn = false;
    m_powerLevel = 0;
}

bool Conditioner::isOn() const {
    return m_isOn;
}

void Conditioner::setPower(int level) {
    if (level < 0 || level > 100) {
        return;
    }
    if (m_isOn) {
        m_powerLevel = level;
    }
}

int Conditioner::getPower() const {
    return m_powerLevel;
}