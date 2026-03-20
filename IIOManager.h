#pragma once
#include "ISensor.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

class IIOManager {
public:
    virtual ~IIOManager() = default;

    // Управление датчиками
    virtual void addSensor(ISensor* sensor) = 0;
    virtual void removeSensor(const std::string& sensorType) = 0;

    // Опрос всех датчиков (тип датчика -> значение)
    virtual std::map<std::string, double> readAllSensors() = 0;
};

