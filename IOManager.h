#pragma once
#include "IIOManager.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

class IOManager : public IIOManager {
private:
    std::map<int, ISensor*> m_sensors_by_id;  // ID -> датчик

public:
    void addSensor(ISensor* sensor) override;
    void removeSensor(int sensorId) override;
    ISensor* getSensor(int sensorId) override;
    std::map<int, double> readAllSensors() override;
};
