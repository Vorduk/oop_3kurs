#include "AlertObserver.h"
#include <iostream>

AlertObserver::AlertObserver(double minTemp, double maxTemp, double maxHumidity)
    : m_minTemp(minTemp), m_maxTemp(maxTemp), m_maxHumidity(maxHumidity) {
}

void AlertObserver::onTemperatureChanged(double oldValue, double newValue) {
    if (newValue < m_minTemp) {
        std::cout << "[AlertObserver] WARNING: Temperature " << newValue
            << " C is below minimum " << m_minTemp << " C" << std::endl;
    }
    if (newValue > m_maxTemp) {
        std::cout << "[AlertObserver] WARNING: Temperature " << newValue
            << " C is above maximum " << m_maxTemp << " C" << std::endl;
    }
}

void AlertObserver::onAirHumidityChanged(double oldValue, double newValue) {
    if (newValue > m_maxHumidity) {
        std::cout << "[AlertObserver] WARNING: Air humidity " << newValue
            << " % is above maximum " << m_maxHumidity << " %" << std::endl;
    }
}

void AlertObserver::onSoilMoistureChanged(double oldValue, double newValue) {
    if (newValue < 10) {
        std::cout << "[AlertObserver] WARNING: Soil Moisture " << newValue
            << " % is below minimum " << 10 << " %" << std::endl;
    }
    if (newValue > 90) {
        std::cout << "[AlertObserver] WARNING: Soil Moisture " << newValue
            << " % is above maximum " << 90 << " %" << std::endl;
    }
}