#include "AirHumidifier.h"

AirHumidifier::AirHumidifier() : m_isOn(false), m_mode(MODE_MEDIUM) {}

void AirHumidifier::turnOn() {
    m_isOn = true;
    m_mode = MODE_MEDIUM;
}

void AirHumidifier::turnOff() {
    m_isOn = false;
    m_mode = MODE_MEDIUM;
}

bool AirHumidifier::isOn() const {
    return m_isOn;
}

void AirHumidifier::setMode(int mode) {
    if (mode < MODE_LOW) {
        m_mode = MODE_LOW;
    }
    if (mode > MODE_HIGH) {
        m_mode = MODE_HIGH;
    }

    m_mode = mode;
}

int AirHumidifier::getMode() const {
    return m_mode;
}