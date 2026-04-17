#pragma once
#include "SensorCreator.h"
#include "AirHumiditySensor.h"

/**
 * @brief Конкретный создатель датчиков влажности воздуха (ConcreteCreator)
 */
class AirHumiditySensorCreator : public SensorCreator {
public:
    std::shared_ptr<ISensor> createSensor(
        std::weak_ptr<const IDataProvider> data_provider) override;
};