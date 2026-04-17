#include "TemperatureLogic.h"
#include <iostream>

TemperatureLogic::TemperatureLogic(std::weak_ptr<const IDataProvider> provider)
    : m_data_provider(provider) {
    std::cout << "[TemperatureLogic]: Created new temperature logic object" << std::endl;
}

double TemperatureLogic::readValue() {
    if (auto provider = m_data_provider.lock()) {
        return provider->getTemperature();
    }
    return 0.0;
}

std::string TemperatureLogic::getType() const {
    return "temperature";
}