#include "Irrigation.h"
#include <stdexcept>

Irrigation::Irrigation() : m_isOn(false), m_mode(MODE_DRIP) {}

void Irrigation::turnOn() {
    m_isOn = true;
    m_mode = MODE_DRIP;  // По умолчанию капельный полив.
}

void Irrigation::turnOff() {
    m_isOn = false;
    m_mode = MODE_DRIP;
}

bool Irrigation::isOn() const {
    return m_isOn;
}

void Irrigation::setMode(int mode) {
    if (mode < MODE_DRIP) {
        m_mode = MODE_DRIP;
    }
    if (mode > MODE_SPRINKLER) {
        m_mode = MODE_SPRINKLER;
    }

    m_mode = mode;
}

int Irrigation::getMode() const {
    return m_mode;
}