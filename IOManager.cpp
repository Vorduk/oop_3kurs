#include "IOManager.h"
#include <algorithm>
#include <iostream>

void IOManager::addSensor(ISensor* sensor) {
    int id = sensor->getId();

    m_sensors_by_id[id] = sensor;

}

void IOManager::removeSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    if (it != m_sensors_by_id.end()) {
        std::string type = it->second->getType();

        m_sensors_by_id.erase(it);
    }
    else {
        std::cout << "[WARNING]: Sensor with ID " << sensorId << " not found" << std::endl;
    }
}

ISensor* IOManager::getSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    if (it != m_sensors_by_id.end()) {
        return it->second;
    }
    return nullptr;
}

std::map<int, double> IOManager::readAllSensors() {
    std::map<int, double> readings;

    for (const auto& [id, sensor] : m_sensors_by_id) {
        readings[id] = sensor->getValue();
    }

    return readings;
}
