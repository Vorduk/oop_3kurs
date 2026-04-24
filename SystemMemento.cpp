#include "SystemMemento.h"

SystemMemento::SystemMemento()
    : m_temperature(0.0), m_airHumidity(0.0), m_soilMoisture(0.0) {
}

SystemMemento::SystemMemento(double temp, double humidity, double soil,
    const std::map<std::string, double>& targets,
    const std::map<std::string, int>& deviceCommands)
    : m_temperature(temp), m_airHumidity(humidity), m_soilMoisture(soil),
    m_targets(targets), m_deviceCommands(deviceCommands) {
}

double SystemMemento::getTemperature() const {
    return m_temperature;
}

double SystemMemento::getAirHumidity() const {
    return m_airHumidity;
}

double SystemMemento::getSoilMoisture() const {
    return m_soilMoisture;
}

const std::map<std::string, double>& SystemMemento::getTargets() const {
    return m_targets;
}

const std::map<std::string, int>& SystemMemento::getDeviceCommands() const {
    return m_deviceCommands;
}