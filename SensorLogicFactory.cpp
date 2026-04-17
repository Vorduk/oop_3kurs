#include "SensorLogicFactory.h"
#include "TemperatureLogic.h"
#include "AirHumidityLogic.h"
#include "SoilMoistureLogic.h"
#include <iostream>

std::shared_ptr<ISensorLogic> SensorLogicFactory::getSensorLogic(
    const std::string& type,
    std::weak_ptr<const IDataProvider> provider) {

    std::string key = makeKey(type, provider);

    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        std::cout << "[SensorLogicFactory]: Returning EXISTING logic for key: " << key << std::endl;
        return it->second;
    }

    std::cout << "[SensorLogicFactory]: Creating NEW logic for key: " << key << std::endl;

    std::shared_ptr<ISensorLogic> logic;
    if (type == "temperature") {
        logic = std::make_shared<TemperatureLogic>(provider);
    }
    else if (type == "air_humidity") {
        logic = std::make_shared<AirHumidityLogic>(provider);
    }
    else if (type == "soil_moisture") {
        logic = std::make_shared<SoilMoistureLogic>(provider);
    }
    else {
        return nullptr;
    }

    m_cache[key] = logic;
    return logic;
}

std::string SensorLogicFactory::makeKey(
    const std::string& type,
    std::weak_ptr<const IDataProvider> provider) {

    std::string providerAddr = "null";
    if (auto p = provider.lock()) {
        providerAddr = std::to_string(reinterpret_cast<uintptr_t>(p.get()));
    }
    return type + "_" + providerAddr;
}