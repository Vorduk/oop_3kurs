#include "AirHumidityLogic.h"
#include <iostream>

AirHumidityLogic::AirHumidityLogic(std::weak_ptr<const IDataProvider> provider)
    : m_data_provider(provider) {
    std::cout << "[AirHumidityLogic]: Created new air humidity logic object" << std::endl;
}

double AirHumidityLogic::readValue() {
    if (auto provider = m_data_provider.lock()) {
        return provider->getAirHumidity();
    }
    return 0.0;
}

std::string AirHumidityLogic::getType() const {
    return "air_humidity";
}