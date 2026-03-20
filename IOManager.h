#pragma once
#include "IIOManager.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class IOManager : public IIOManager {
public:
    // Управление датчиками
    void addSensor(std::shared_ptr<ISensor> sensor) override;
    void removeSensor(int sensorId) override;
    std::shared_ptr<ISensor> getSensor(int sensorId) override;
    std::map<int, double> readAllSensors() override;

    // Управление устройствами
    void addDevice(std::shared_ptr<IDevice> device) override;
    void removeDevice(int deviceId) override;
    std::shared_ptr<IDevice> getDevice(int deviceId) override;

    // Команды управления устройствами
    void sendOnOffCommand(int deviceId, bool turnOn) override;
    void sendPowerCommand(int deviceId, int powerLevel) override;
    void sendModeCommand(int deviceId, int mode) override;

    // Вспомогательный метод для получения устройств по типу
    std::vector<int> getDeviceIdsByType(const std::string& type);

private:
    std::map<int, std::shared_ptr<ISensor>> m_sensors_by_id;
    std::map<int, std::shared_ptr<IDevice>> m_devices_by_id;
    std::map<std::string, std::vector<int>> m_devices_by_type;
};