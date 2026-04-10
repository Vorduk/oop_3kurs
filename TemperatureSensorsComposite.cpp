// AllTemperatureSensors.cpp
#include "TemperatureSensorsComposite.h"
#include <iostream>
#include <algorithm>

TemperatureSensorsComposite::TemperatureSensorsComposite()
    : BaseSensor(std::weak_ptr<const IDataProvider>(), "temperature") {
    // DataProvider пустой, т. к. он не нужен для композитного датчика
}

void TemperatureSensorsComposite::addSensor(std::shared_ptr<ISensor> sensor) {
    if (sensor && sensor->getType() == "temperature") {
        m_sensors.push_back(sensor);
        std::cout << "[TemperatureSensorsComposite]: Added temperature sensor ID:"
            << sensor->getId() << " to composite ID:" << getId() << std::endl;
    }
}

void TemperatureSensorsComposite::removeSensor(int sensorId) {
    auto it = std::remove_if(m_sensors.begin(), m_sensors.end(),
        [sensorId](const std::shared_ptr<ISensor>& s) {
            return s && s->getId() == sensorId;
        });

    if (it != m_sensors.end()) {
        std::cout << "[TemperatureSensorsComposite]: Removed temperature sensor ID:"
            << sensorId << " from composite ID:" << getId() << std::endl;
        m_sensors.erase(it, m_sensors.end());
    }
}

std::shared_ptr<ISensor> TemperatureSensorsComposite::getSensor(int sensorId) const {
    for (const auto& sensor : m_sensors) {
        if (sensor && sensor->getId() == sensorId) {
            return sensor;
        }
    }
    return nullptr;
}

double TemperatureSensorsComposite::getValue() {
    if (m_sensors.empty()) {
        std::cout << "[TemperatureSensorsComposite]: No sensors in composite ID:" << getId() << std::endl;
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& sensor : m_sensors) {
        if (sensor) {
            sum += sensor->getValue();
        }
    }

    double average = sum / m_sensors.size();
    std::cout << "[TemperatureSensorsComposite]: " << getType() << " [ID:" << getId()
        << "] average = " << average << " (from "
        << m_sensors.size() << " sensors)" << std::endl;

    return average;
}