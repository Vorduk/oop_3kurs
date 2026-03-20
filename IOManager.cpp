#include "IOManager.h"
#include <algorithm>
#include <iostream>

// ==================== Управление датчиками ====================

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
    return (it != m_sensors_by_id.end()) ? it->second : nullptr;
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

// ==================== Управление устройствами ====================

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
    return (it != m_devices_by_id.end()) ? it->second : nullptr;
}

std::vector<int> IOManager::getDeviceIdsByType(const std::string& type) {
    auto it = m_devices_by_type.find(type);
    return (it != m_devices_by_type.end()) ? it->second : std::vector<int>();
}

// ==================== Команды управления устройствами ====================

void IOManager::sendOnOffCommand(int deviceId, bool turnOn) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[WARNING]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    if (turnOn && !device->isOn()) {
        device->turnOn();
        std::cout << "[CMD]: Device " << deviceId << " (" << device->getType()
            << ") turned ON" << std::endl;
    }
    else if (!turnOn && device->isOn()) {
        device->turnOff();
        std::cout << "[CMD]: Device " << deviceId << " (" << device->getType()
            << ") turned OFF" << std::endl;
    }
}

void IOManager::sendPowerCommand(int deviceId, int powerLevel) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[WARNING]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    // Убеждаемся, что устройство включено перед установкой мощности
    if (powerLevel > 0 && !device->isOn()) {
        sendOnOffCommand(deviceId, true);
    }

    auto adjustable = std::dynamic_pointer_cast<IAdjustableDevice>(device);
    if (adjustable && powerLevel > 0) {
        adjustable->setPower(powerLevel);
        std::cout << "[CMD]: Device " << deviceId << " power set to "
            << powerLevel << "%" << std::endl;
    }
    else if (powerLevel <= 0 && device->isOn()) {
        sendOnOffCommand(deviceId, false);
    }
}

void IOManager::sendModeCommand(int deviceId, int mode) {
    auto device = getDevice(deviceId);
    if (!device) {
        std::cout << "[WARNING]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    auto modeSelectable = std::dynamic_pointer_cast<IModeSelectableDevice>(device);
    if (modeSelectable) {
        modeSelectable->setMode(mode);
        std::cout << "[CMD]: Device " << deviceId << " mode set to "
            << mode << std::endl;
    }
}