#include <iostream>
#include <memory>
#include "SignalProcessor.h"

int main() {
    std::cout << "Chain of Responsibility" << std::endl;

    SignalProcessor processor;

    std::cout << std::endl;
    std::cout << "Test 1: Temperature too low" << std::endl;
    processor.process_signal("temperature", 15.0);

    std::cout << std::endl;
    std::cout << "Test 2: Temperature too high" << std::endl;
    processor.process_signal("temperature", 32.0);

    std::cout << std::endl;
    std::cout << "Test 3: Temperature normal" << std::endl;
    processor.process_signal("temperature", 23.0);

    std::cout << std::endl;
    std::cout << "Test 4: Air humidity too low" << std::endl;
    processor.process_signal("air_humidity", 40.0);

    std::cout << std::endl;
    std::cout << "Test 5: Air humidity too high" << std::endl;
    processor.process_signal("air_humidity", 85.0);

    std::cout << std::endl;
    std::cout << "Test 6: Soil moisture too low" << std::endl;
    processor.process_signal("soil_moisture", 25.0);

    std::cout << std::endl;
    std::cout << "Test 7: Soil moisture too high" << std::endl;
    processor.process_signal("soil_moisture", 75.0);

    std::cout << std::endl;
    std::cout << "Test 8: Soil moisture normal" << std::endl;
    processor.process_signal("soil_moisture", 50.0);

    std::cout << std::endl;
    std::cout << "Test 9: Unknown signal" << std::endl;
    processor.process_signal("light_intensity", 500.0);

    return 0;
}