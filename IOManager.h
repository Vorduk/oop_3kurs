#pragma once
#include "IIOManager.h"
#include <map>
#include <string>
#include <vector>

class IOManager : public IIOManager {
private:
    std::map<int, ISensor*> m_sensors_by_id;
    std::map<int, IDevice*> m_devices_by_id;
    std::map<std::string, std::vector<int>> m_devices_by_type;

public:
    void addSensor(ISensor* sensor) override;
    void removeSensor(int sensorId) override;
    ISensor* getSensor(int sensorId) override;
    std::map<int, double> readAllSensors() override;

    void addDevice(IDevice* device) override;
    void removeDevice(int deviceId) override;
    IDevice* getDevice(int deviceId) override;
    void sendCommand(int deviceId, int powerLevel) override;
    void sendCommand(int deviceId, int mode, bool isModeCommand) override;

    std::vector<int> getDeviceIdsByType(const std::string& type);
};