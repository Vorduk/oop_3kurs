#include "Ventilation.h"

Ventilation::Ventilation() : m_isOn(false), m_mode(MODE_MEDIUM) {}

void Ventilation::turnOn() {
    m_isOn = true;
    // Cредний режим по умолчанию
    m_mode = MODE_MEDIUM;
}

void Ventilation::turnOff() {
    m_isOn = false;
    m_mode = MODE_MEDIUM;
}

bool Ventilation::isOn() const {
    return m_isOn;
}

void Ventilation::setMode(int mode) {
    if (mode < MODE_LOW) {
        m_mode = MODE_LOW;
    }
    if (mode > MODE_HIGH) {
        m_mode = MODE_HIGH;
    }

    m_mode = mode;
}

int Ventilation::getMode() const {
    return m_mode;
}