#include "TemperatureSensorCreator.h"
#include <iostream>

std::shared_ptr<ISensor> TemperatureSensorCreator::createSensor(
    std::weak_ptr<const IDataProvider> data_provider) {

    auto sensor = std::make_shared<TemperatureSensor>(data_provider);
    std::cout << "[FactoryMethod]: TemperatureSensorCreator created sensor ID="
        << sensor->getId() << std::endl;
    return sensor;
}