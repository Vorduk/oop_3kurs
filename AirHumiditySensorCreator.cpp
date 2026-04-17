#include "AirHumiditySensorCreator.h"
#include <iostream>

std::shared_ptr<ISensor> AirHumiditySensorCreator::createSensor(
    std::weak_ptr<const IDataProvider> data_provider) {

    auto sensor = std::make_shared<AirHumiditySensor>(data_provider);
    std::cout << "[FactoryMethod]: AirHumiditySensorCreator created sensor ID="
        << sensor->getId() << std::endl;
    return sensor;
}