#pragma once
#include "IIOManager.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class IOManager : public IIOManager {
public:
    void addSensor(std::shared_ptr<ISensor> sensor) override;
    void removeSensor(int sensorId) override;
    std::shared_ptr<ISensor> getSensor(int sensorId) override;
    std::map<int, double> readAllSensors() override;

    void addDevice(std::shared_ptr<IDevice> device) override;
    void removeDevice(int deviceId) override;
    std::shared_ptr<IDevice> getDevice(int deviceId) override;
    void sendCommand(int deviceId, int powerLevel) override;
    void sendCommand(int deviceId, int mode, bool isModeCommand) override;

    // Дополнительный метод для получения устройств по типу
    std::vector<int> getDeviceIdsByType(const std::string& type);

private:
    std::map<int, std::shared_ptr<ISensor>> m_sensors_by_id;
    std::map<int, std::shared_ptr<IDevice>> m_devices_by_id;
    std::map<std::string, std::vector<int>> m_devices_by_type;
};