#pragma once
#include "BaseHandler.h"
#include <iostream>

class HumidityHandler : public BaseHandler {
public:
    std::string handle(const std::string& signal, double value) override {
        std::cout << "[HumidityHandler]: Received signal '" << signal << "' with value " << value << std::endl;

        if (signal == "air_humidity") {
            if (value < 50.0) {
                std::cout << "[HumidityHandler]: Humidity too low, activating humidifier" << std::endl;
                return "HUMIDIFIER_ON";
            }
            if (value > 80.0) {
                std::cout << "[HumidityHandler]: Humidity too high, activating ventilation" << std::endl;
                return "VENTILATION_ON";
            }
            std::cout << "[HumidityHandler]: Humidity is normal" << std::endl;
            return "HUMIDITY_OK";
        }

        std::cout << "[HumidityHandler]: Cannot handle signal '" << signal << "'" << std::endl;
        return pass_to_next(signal, value);
    }
};