#include "SoilMoistureLogic.h"
#include <iostream>

SoilMoistureLogic::SoilMoistureLogic(std::weak_ptr<const IDataProvider> provider)
    : m_data_provider(provider) {
    std::cout << "[SoilMoistureLogic]: Created new soil moisture logic object" << std::endl;
}

double SoilMoistureLogic::readValue() {
    if (auto provider = m_data_provider.lock()) {
        return provider->getSoilMoisture();
    }
    return 0.0;
}

std::string SoilMoistureLogic::getType() const {
    return "soil_moisture";
}