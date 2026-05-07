#pragma once
#include "BaseHandler.h"
#include <iostream>

class SoilMoistureHandler : public BaseHandler {
public:
    std::string handle(const std::string& signal, double value) override {
        std::cout << "[SoilMoistureHandler]: Received signal '" << signal << "' with value " << value << std::endl;

        if (signal == "soil_moisture") {
            if (value < 35.0) {
                std::cout << "[SoilMoistureHandler]: Soil moisture too low, activating irrigation" << std::endl;
                return "IRRIGATION_ON";
            }
            if (value > 70.0) {
                std::cout << "[SoilMoistureHandler]: Soil moisture too high, stopping irrigation" << std::endl;
                return "IRRIGATION_OFF";
            }
            std::cout << "[SoilMoistureHandler]: Soil moisture is normal" << std::endl;
            return "SOIL_MOISTURE_OK";
        }

        std::cout << "[SoilMoistureHandler]: Cannot handle signal '" << signal << "'" << std::endl;
        return pass_to_next(signal, value);
    }
};