#include "IOManager.h"
#include <algorithm>
#include <iostream>

void IOManager::addSensor(std::shared_ptr<ISensor> sensor) {
    int id = sensor->getId();
    m_sensors_by_id[id] = sensor;
    std::cout << "[INFO]: Sensor added - ID: " << id
        << ", Type: " << sensor->getType() << std::endl;
}

void IOManager::removeSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    if (it != m_sensors_by_id.end()) {
        m_sensors_by_id.erase(it);
        std::cout << "[INFO]: Sensor removed - ID: " << sensorId << std::endl;
    }
    else {
        std::cout << "[WARNING]: Sensor with ID " << sensorId << " not found" << std::endl;
    }
}

std::shared_ptr<ISensor> IOManager::getSensor(int sensorId) {
    auto it = m_sensors_by_id.find(sensorId);
    if (it != m_sensors_by_id.end()) {
        return it->second;
    }
    return nullptr;
}

std::map<int, double> IOManager::readAllSensors() {
    std::map<int, double> readings;
    for (const auto& [id, sensor] : m_sensors_by_id) {
        if (sensor) {
            readings[id] = sensor->getValue();
        }
    }
    return readings;
}

void IOManager::addDevice(std::shared_ptr<IDevice> device) {
    int id = device->getId();
    std::string type = device->getType();

    m_devices_by_id[id] = device;
    m_devices_by_type[type].push_back(id);

    std::cout << "[INFO]: Device added - ID: " << id
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
        std::cout << "[INFO]: Device removed - ID: " << deviceId << std::endl;
    }
    else {
        std::cout << "[WARNING]: Device with ID " << deviceId << " not found" << std::endl;
    }
}

std::shared_ptr<IDevice> IOManager::getDevice(int deviceId) {
    auto it = m_devices_by_id.find(deviceId);
    if (it != m_devices_by_id.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<int> IOManager::getDeviceIdsByType(const std::string& type) {
    auto it = m_devices_by_type.find(type);
    if (it != m_devices_by_type.end()) {
        return it->second;
    }
    return std::vector<int>();
}

void IOManager::sendCommand(int deviceId, int powerLevel) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[WARNING]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    if (powerLevel > 0 && !device->isOn()) {
        device->turnOn();
        std::cout << "[CMD]: Device " << deviceId << " (" << device->getType()
            << ") turned ON" << std::endl;
    }
    else if (powerLevel <= 0 && device->isOn()) {
        device->turnOff();
        std::cout << "[CMD]: Device " << deviceId << " (" << device->getType()
            << ") turned OFF" << std::endl;
    }

    auto adjustable = std::dynamic_pointer_cast<IAdjustableDevice>(device);
    if (adjustable && powerLevel > 0) {
        adjustable->setPower(powerLevel);
        std::cout << "[CMD]: Device " << deviceId << " power set to "
            << powerLevel << "%" << std::endl;
    }
}

void IOManager::sendCommand(int deviceId, int mode, bool isModeCommand) {
    if (!isModeCommand) return;

    auto device = getDevice(deviceId);
    if (!device) return;

    auto modeSelectable = std::dynamic_pointer_cast<IModeSelectableDevice>(device);
    if (modeSelectable) {
        modeSelectable->setMode(mode);
        std::cout << "[CMD]: Device " << deviceId << " mode set to "
            << mode << std::endl;
    }
}