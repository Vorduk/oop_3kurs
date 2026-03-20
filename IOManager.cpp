#include "IOManager.h"
#include <algorithm>
#include <iostream>

void IOManager::addSensor(ISensor* sensor) {
    m_sensors.push_back(sensor);
    std::cout << "[INFO]: Added sensor: " << sensor->getType() << std::endl;
}

void IOManager::removeSensor(const std::string& sensorType) {
    m_sensors.erase(
        std::remove_if(m_sensors.begin(), m_sensors.end(),
            [&sensorType](ISensor* sensor) {
                return sensor->getType() == sensorType;
            }),
        m_sensors.end()
    );
    std::cout << "[INFO]: Removed sensors of type: " << sensorType << std::endl;
}

std::map<std::string, double> IOManager::readAllSensors() {
    std::map<std::string, double> readings;

    for (std::vector<ISensor*>::const_iterator it = m_sensors.begin();
        it != m_sensors.end(); ++it) {
        readings[(*it)->getType()] = (*it)->getValue();
    }

    return readings;
}