#pragma once
#include "ISensor.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "IDevice.h"

class IIOManager {
public:
    virtual ~IIOManager() = default;

    // Управление датчиками
    virtual void addSensor(ISensor* sensor) = 0;
    virtual void removeSensor(int sensorId) = 0;  // Удаление по ID
    virtual ISensor* getSensor(int sensorId) = 0;
    virtual std::map<int, double> readAllSensors() = 0;

    // Управление устройствами
    virtual void addDevice(IDevice* device) = 0;
    virtual void removeDevice(int deviceId) = 0;
    virtual IDevice* getDevice(int deviceId) = 0;
    virtual void sendCommand(int deviceId, int powerLevel) = 0;
    virtual void sendCommand(int deviceId, int mode, bool isModeCommand) = 0;
    
};
