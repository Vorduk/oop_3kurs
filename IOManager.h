#pragma once
#include "IIOManager.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

class IOManager : public IIOManager {
private:
    std::vector<ISensor*> m_sensors;

public:
    void addSensor(ISensor* sensor) override;
    void removeSensor(const std::string& sensorType) override;
    std::map<std::string, double> readAllSensors() override;
};

