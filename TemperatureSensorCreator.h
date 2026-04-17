#pragma once
#include "SensorCreator.h"
#include "TemperatureSensor.h"

/**
 * @brief Конкретный создатель датчиков температуры (ConcreteCreator)
 */
class TemperatureSensorCreator : public SensorCreator {
public:
    std::shared_ptr<ISensor> createSensor(
        std::weak_ptr<const IDataProvider> data_provider) override;
};