#pragma once
#include <vector>
#include <memory>
#include "BaseSensor.h"


class TemperatureSensorsComposite : public BaseSensor {
private:
    std::vector<std::shared_ptr<ISensor>> m_sensors;

public:
    TemperatureSensorsComposite();

    // Методы управления композицией
    void addSensor(std::shared_ptr<ISensor> sensor);
    void removeSensor(int sensorId);
    std::shared_ptr<ISensor> getSensor(int sensorId) const;

    // Реализация виртуального метода BaseSensor
    double getValue() override;
};