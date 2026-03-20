#include "IOManager.h"
#include <algorithm>
#include <iostream>

void IOManager::addSensor(ISensor* sensor) {
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

void IOManager::addDevice(IDevice* device) {
    int id = device->getId();
    m_devices_by_id[id] = device;
    std::cout << "[INFO]: Device added - ID: " << id
        << ", Type: " << device->getType() << std::endl;
}

void IOManager::removeDevice(int deviceId) {
    auto it = m_devices_by_id.find(deviceId);
    if (it != m_devices_by_id.end()) {
        m_devices_by_id.erase(it);
        std::cout << "[INFO]: Device removed - ID: " << deviceId << std::endl;
    }
    else {
        std::cout << "[WARNING]: Device with ID " << deviceId << " not found" << std::endl;
    }
}

IDevice* IOManager::getDevice(int deviceId) {
    auto it = m_devices_by_id.find(deviceId);
    if (it != m_devices_by_id.end()) {
        return it->second;
    }
    return nullptr;
}

void IOManager::sendCommand(int deviceId, int powerLevel) {
    auto* device = getDevice(deviceId);
    if (!device) {
        std::cout << "[WARNING]: Device " << deviceId << " not found" << std::endl;
        return;
    }

    // Включаем/выключаем устройство
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

    // Устанавливаем мощность, если устройство поддерживает
    auto* adjustable = dynamic_cast<IAdjustableDevice*>(device);
    if (adjustable && powerLevel > 0) {
        adjustable->setPower(powerLevel);
        std::cout << "[CMD]: Device " << deviceId << " power set to "
            << powerLevel << "%" << std::endl;
    }
}

void IOManager::sendCommand(int deviceId, int mode, bool isModeCommand) {
    if (!isModeCommand) return;

    auto* device = getDevice(deviceId);
    if (!device) return;

    // Устанавливаем режим, если устройство поддерживает
    auto* modeSelectable = dynamic_cast<IModeSelectableDevice*>(device);
    if (modeSelectable) {
        modeSelectable->setMode(mode);
        std::cout << "[CMD]: Device " << deviceId << " mode set to "
            << mode << std::endl;
    }
}
