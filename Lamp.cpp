#include "Lamp.h"

Lamp::Lamp()
    : BaseDevice("lamp"), m_isOn(false), m_powerLevel(0), m_mode(MODE_WHITE) {
}

void Lamp::turnOn() {
    m_isOn = true;
}

void Lamp::turnOff() {
    m_isOn = false;
}

bool Lamp::isOn() const {
    return m_isOn;
}

void Lamp::setPower(int level) {
    if (level < 0 || level > 100) {
        return;
    }
    if (m_isOn) {
        m_powerLevel = level;
    }
}

int Lamp::getPower() const {
    return m_powerLevel;
}

void Lamp::setMode(int mode) {
    if (mode < MODE_WHITE) {
        m_mode = MODE_WHITE;
    }
    if (mode > MODE_UF) {
        m_mode = MODE_UF;
    }
    m_mode = mode;
}

int Lamp::getMode() const {
    return m_mode;
}