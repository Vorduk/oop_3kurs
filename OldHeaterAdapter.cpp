// OldHeaterAdapter.cpp
#include "OldHeaterAdapter.h"

OldHeaterAdapter::OldHeaterAdapter(std::shared_ptr<OldHeater> legacy)
    : BaseDevice("heater")
    , m_legacy(legacy)
    , m_is_on(false)
    , m_powerLevel(0) {
}

void OldHeaterAdapter::turnOn() {
    if (m_legacy) {
        m_legacy->powerOn();
        m_is_on = true;
    }
}

void OldHeaterAdapter::turnOff() {
    if (m_legacy) {
        m_legacy->powerOff();
        m_is_on = false;
        m_powerLevel = 0;
    }
}

bool OldHeaterAdapter::isOn() const {
    return m_is_on;
}

void OldHeaterAdapter::setPower(int level) {
    if (!m_legacy) return;
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    m_powerLevel = level;
    if (level > 0 && !m_is_on) turnOn();

    // Преобразование 0-100 в 0-10 для OldHeater
    int legacyLevel = level / 10;
    m_legacy->setHeatLevel(legacyLevel);
}

int OldHeaterAdapter::getPower() const {
    return m_powerLevel;
}