#include "LoggingObserver.h"
#include <iostream>

LoggingObserver::LoggingObserver() {
}

void LoggingObserver::onTemperatureChanged(double oldValue, double newValue) {
    std::cout << "[LoggingObserver] Temperature changed: "
        << oldValue << " -> " << newValue << " C" << std::endl;
}

void LoggingObserver::onAirHumidityChanged(double oldValue, double newValue) {
    std::cout << "[LoggingObserver] Air humidity changed: "
        << oldValue << " -> " << newValue << " %" << std::endl;
}

void LoggingObserver::onSoilMoistureChanged(double oldValue, double newValue) {
    std::cout << "[LoggingObserver] Soil moisture changed: "
        << oldValue << " -> " << newValue << " %" << std::endl;
}