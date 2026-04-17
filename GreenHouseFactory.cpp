#include "GreenhouseFactory.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"
#include <iostream>

GreenhouseFactory::GreenhouseFactory() {
    std::cout << "[GreenhouseFactory]: Created factory" << std::endl;
}

std::shared_ptr<IDevice> GreenhouseFactory::createHeater() {
    std::cout << "[GreenhouseFactory]: Creating Heater" << std::endl;
    return std::make_shared<Heater>();
}

std::shared_ptr<IDevice> GreenhouseFactory::createConditioner() {
    std::cout << "[GreenhouseFactory]: Creating Conditioner" << std::endl;
    return std::make_shared<Conditioner>();
}

std::shared_ptr<IDevice> GreenhouseFactory::createAirHumidifier() {
    std::cout << "[GreenhouseFactory]: Creating AirHumidifier" << std::endl;
    return std::make_shared<AirHumidifier>();
}

std::shared_ptr<IDevice> GreenhouseFactory::createIrrigation() {
    std::cout << "[GreenhouseFactory]: Creating Irrigation" << std::endl;
    return std::make_shared<Irrigation>();
}

std::shared_ptr<IDevice> GreenhouseFactory::createVentilation() {
    std::cout << "[GreenhouseFactory]: Creating Ventilation" << std::endl;
    return std::make_shared<Ventilation>();
}

std::shared_ptr<IDevice> GreenhouseFactory::createLamp() {
    std::cout << "[GreenhouseFactory]: Creating Lamp" << std::endl;
    return std::make_shared<Lamp>();
}

std::shared_ptr<IRegulator> GreenhouseFactory::createTemperatureRegulator() {
    std::cout << "[GreenhouseFactory]: Creating PID regulator for temperature" << std::endl;
    return std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
}

std::shared_ptr<IRegulator> GreenhouseFactory::createHumidityRegulator() {
    std::cout << "[GreenhouseFactory]: Creating OnOff regulator for humidity" << std::endl;
    return std::make_shared<OnOffRegulator>(2.0);
}

std::shared_ptr<IRegulator> GreenhouseFactory::createSoilMoistureRegulator() {
    std::cout << "[GreenhouseFactory]: Creating OnOff regulator for soil moisture" << std::endl;
    return std::make_shared<OnOffRegulator>(3.0);
}