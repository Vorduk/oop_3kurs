#pragma once
#include "SensorCreator.h"
#include "SoilMoistureSensor.h"

/**
 * @brief Конкретный создатель датчиков влажности почвы (ConcreteCreator)
 */
class SoilMoistureSensorCreator : public SensorCreator {
public:
    std::shared_ptr<ISensor> createSensor(
        std::weak_ptr<const IDataProvider> data_provider) override;
};