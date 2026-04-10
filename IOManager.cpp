#include "IOManager.h"
#include <algorithm>
#include <iostream>

void IOManager::addSensor(std::shared_ptr<ISensor> sensor) {
    int id = sensor->getId();
    std::string type = sensor->getType();

    m_sensors_by_id[id] = sensor;
    m_sensors_by_type[type].push_back(sensor);

    std::cout << "[IOManager] Sensor added - ID: " << id
        << ", Type: " << type
        << " (now " << m_sensors_by_type[type].size() << " sensors of this type)" << std::endl;
}

void IOManager::removeSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    if (it != m_sensors_by_id.end()) {
        std::string type = it->second->getType();

        // Remove from type map
        auto& vec = m_sensors_by_type[type];
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [sensorId](const std::shared_ptr<ISensor>& s) {
                return s->getId() == sensorId;
            }), vec.end());

        if (vec.empty()) {
            m_sensors_by_type.erase(type);
        }

        m_sensors_by_id.erase(it);
        std::cout << "[IOManager] Sensor removed - ID: " << sensorId << std::endl;
    }
}

std::shared_ptr<ISensor> IOManager::getSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    return (it != m_sensors_by_id.end()) ? it->second : nullptr;
}

std::map<int, double> IOManager::readAllSensors() {
    std::map<int, double> readings;

    std::cout << "[IOManager] Reading all " << m_sensors_by_id.size() << " sensors (by ID)..." << std::endl;

    for (const auto& [id, sensor] : m_sensors_by_id) {
        if (sensor) {
            double val = sensor->getValue();
            readings[id] = val;
            std::cout << "[IOManager] Sensor ID:" << id
                << " (" << sensor->getType() << ") = " << val << std::endl;
        }
    }

    return readings;
}

std::map<std::string, double> IOManager::readAggregatedSensors() {
    std::map<std::string, double> aggregated;

    std::cout << "[IOManager] Reading aggregated sensors by type..." << std::endl;

    for (const auto& [type, sensors] : m_sensors_by_type) {
        if (sensors.empty()) continue;

        double sum = 0.0;
        for (const auto& sensor : sensors) {
            if (sensor) {
                double val = sensor->getValue();
                sum += val;
                std::cout << "[IOManager]   " << type << " sensor ID:" << sensor->getId()
                    << " = " << val << std::endl;
            }
        }

        double average = sum / sensors.size();
        aggregated[type] = average;

        std::cout << "[IOManager]   Aggregated " << type << " = " << average
            << " (from " << sensors.size() << " sensors)" << std::endl;
    }

    return aggregated;
}

std::vector<std::shared_ptr<ISensor>> IOManager::getSensorsByType(const std::string& type) {
    auto it = m_sensors_by_type.find(type);
    return (it != m_sensors_by_type.end()) ? it->second : std::vector<std::shared_ptr<ISensor>>();
}

void IOManager::addDevice(std::shared_ptr<IDevice> device) {
    int id = device->getId();
    std::string type = device->getType();

    m_devices_by_id[id] = device;
    m_devices_by_type[type].push_back(id);

    std::cout << "[IOManager]: Device added - ID: " << id
        << ", Type: " << type << std::endl;
}

void IOManager::removeDevice(int deviceId) {
    auto it = m_devices_by_id.find(deviceId);
    if (it != m_devices_by_id.end()) {
        std::string type = it->second->getType();

        auto typeIt = m_devices_by_type.find(type);
        if (typeIt != m_devices_by_type.end()) {
            auto& ids = typeIt->second;
            ids.erase(std::remove(ids.begin(), ids.end(), deviceId), ids.end());
            if (ids.empty()) {
                m_devices_by_type.erase(typeIt);
            }
        }

        m_devices_by_id.erase(it);
        std::cout << "[IOManager]: Device removed - ID: " << deviceId << std::endl;
    }
    else {
        std::cout << "[IOManager]: Device with ID " << deviceId << " not found" << std::endl;
    }
}

std::shared_ptr<IDevice> IOManager::getDevice(int deviceId) {
    auto it = m_devices_by_id.find(deviceId);
    return (it != m_devices_by_id.end()) ? it->second : nullptr;
}

std::vector<int> IOManager::getDeviceIdsByType(const std::string& type) {
    auto it = m_devices_by_type.find(type);
    return (it != m_devices_by_type.end()) ? it->second : std::vector<int>();
}

void IOManager::sendOnOffCommand(int deviceId, bool turnOn) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << " not found" << std::endl;
        return;
    }

    if (turnOn && !device->isOn()) {
        device->turnOn();
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << ") turned ON" << std::endl;
    }
    else if (!turnOn && device->isOn()) {
        device->turnOff();
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << ") turned OFF" << std::endl;
    }
}

void IOManager::sendPowerCommand(int deviceId, int powerLevel) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[IOManager]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    // Убеждаемся, что устройство включено перед установкой мощности
    if (powerLevel > 0 && !device->isOn()) {
        sendOnOffCommand(deviceId, true);
    }

    auto adjustable = std::dynamic_pointer_cast<IAdjustableDevice>(device);
    if (adjustable && powerLevel > 0) {
        adjustable->setPower(powerLevel);
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << " power set to "
            << powerLevel << "%" << std::endl;
    }
    else if (powerLevel <= 0 && device->isOn()) {
        sendOnOffCommand(deviceId, false);
    }
}

void IOManager::sendModeCommand(int deviceId, int mode) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << " not found" << std::endl;
        return;
    }

    auto modeSelectable = std::dynamic_pointer_cast<IModeSelectableDevice>(device);
    if (modeSelectable) {
        modeSelectable->setMode(mode);
        std::cout << "[IOManager]: Device " << deviceId << " (" << device->getType()
            << " mode set to "
            << mode << std::endl;
    }
}