#pragma once
#include "ISensor.h"
#include "IDevice.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

class IIOManager {
public:
    virtual ~IIOManager() = default;

    // Управление датчиками - теперь принимаем shared_ptr
    virtual void addSensor(std::shared_ptr<ISensor> sensor) = 0;
    virtual void removeSensor(int sensorId) = 0;
    virtual std::shared_ptr<ISensor> getSensor(int sensorId) = 0;
    virtual std::map<int, double> readAllSensors() = 0;

    // Управление устройствами
    virtual void addDevice(std::shared_ptr<IDevice> device) = 0;
    virtual void removeDevice(int deviceId) = 0;
    virtual std::shared_ptr<IDevice> getDevice(int deviceId) = 0;
    virtual void sendCommand(int deviceId, int powerLevel) = 0;
    virtual void sendCommand(int deviceId, int mode, bool isModeCommand) = 0;
};