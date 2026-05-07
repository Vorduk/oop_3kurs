#pragma once
#include "BaseHandler.h"
#include <iostream>

class TemperatureHandler : public BaseHandler {
public:
    std::string handle(const std::string& signal, double value) override {
        std::cout << "[TemperatureHandler]: Received signal '" << signal << "' with value " << value << std::endl;

        if (signal == "temperature") {
            if (value < 18.0) {
                std::cout << "[TemperatureHandler]: Temperature too low, activating heater" << std::endl;
                return "HEATER_ON";
            }
            if (value > 28.0) {
                std::cout << "[TemperatureHandler]: Temperature too high, activating conditioner" << std::endl;
                return "CONDITIONER_ON";
            }
            std::cout << "[TemperatureHandler]: Temperature is normal" << std::endl;
            return "TEMPERATURE_OK";
        }

        std::cout << "[TemperatureHandler]: Cannot handle signal '" << signal << "'" << std::endl;
        return pass_to_next(signal, value);
    }
};