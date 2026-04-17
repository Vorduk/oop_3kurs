#include "SoilMoistureSensorCreator.h"
#include <iostream>

std::shared_ptr<ISensor> SoilMoistureSensorCreator::createSensor(
    std::weak_ptr<const IDataProvider> data_provider) {

    auto sensor = std::make_shared<SoilMoistureSensor>(data_provider);
    std::cout << "[FactoryMethod]: SoilMoistureSensorCreator created sensor ID="
        << sensor->getId() << std::endl;
    return sensor;
}