// SignalProcessor.h
#pragma once
#include "TemperatureHandler.h"
#include "HumidityHandler.h"
#include "SoilMoistureHandler.h"
#include "DefaultHandler.h"
#include <iostream>
#include <memory>

class SignalProcessor {
private:
    std::shared_ptr<IHandler> m_chain;

public:
    SignalProcessor() {
        std::cout << "[SignalProcessor]: Building handler chain" << std::endl;

        std::shared_ptr<TemperatureHandler> temp_handler = std::make_shared<TemperatureHandler>();
        std::shared_ptr<HumidityHandler> humidity_handler = std::make_shared<HumidityHandler>();
        std::shared_ptr<SoilMoistureHandler> soil_handler = std::make_shared<SoilMoistureHandler>();
        std::shared_ptr<DefaultHandler> default_handler = std::make_shared<DefaultHandler>();

        temp_handler->set_next(humidity_handler);
        humidity_handler->set_next(soil_handler);
        soil_handler->set_next(default_handler);

        m_chain = temp_handler;

        std::cout << "[SignalProcessor]: Chain built successfully" << std::endl;
    }

    std::string process_signal(const std::string& signal, double value) {
        std::cout << std::endl;
        std::cout << "[SignalProcessor]: Processing signal '" << signal << "' with value " << value << std::endl;
        std::string result = m_chain->handle(signal, value);
        std::cout << "[SignalProcessor]: Final command: " << result << std::endl;
        return result;
    }
};