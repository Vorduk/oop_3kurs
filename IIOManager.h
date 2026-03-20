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
    virtual void removeSensor(int sensorId) = 0;  // Удаление по ID
    virtual ISensor* getSensor(int sensorId) = 0;

    // Опрос всех датчиков (ID датчика -> значение)
    virtual std::map<int, double> readAllSensors() = 0;
};
