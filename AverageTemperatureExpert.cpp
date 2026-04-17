#include "AverageTemperatureExpert.h"
#include "IOManager.h"
#include <iostream>

AverageTemperatureExpert::AverageTemperatureExpert(std::shared_ptr<IIOManager> io_manager)
    : m_io_manager(io_manager) {
}

double AverageTemperatureExpert::getValue() {
    if (!m_io_manager) {
        std::cout << "[AverageTemperatureExpert]: No IOManager provided" << std::endl;
        return 0.0;
    }

    std::shared_ptr<IOManager> concreteManager =
        std::dynamic_pointer_cast<IOManager>(m_io_manager);

    if (!concreteManager) {
        std::cout << "[AverageTemperatureExpert]: Cannot cast to IOManager" << std::endl;
        return 0.0;
    }

    std::vector<std::shared_ptr<ISensor>> sensors =
        concreteManager->getSensorsByType("temperature");

    if (sensors.empty()) {
        std::cout << "[AverageTemperatureExpert]: No temperature sensors found" << std::endl;
        return 0.0;
    }

    double sum = 0.0;
    int count = 0;

    std::cout << "[AverageTemperatureExpert]: Calculating average from "
        << sensors.size() << " sensor(s)..." << std::endl;

    for (size_t i = 0; i < sensors.size(); ++i) {
        std::shared_ptr<ISensor> sensor = sensors[i];
        if (sensor) {
            double value = sensor->getValue();
            sum += value;
            count++;
            std::cout << "[AverageTemperatureExpert]:   Sensor ID:" << sensor->getId()
                << " = " << value << " C" << std::endl;
        }
    }

    if (count == 0) {
        std::cout << "[AverageTemperatureExpert]: No valid readings" << std::endl;
        return 0.0;
    }

    double average = sum / count;
    std::cout << "[AverageTemperatureExpert]: Average temperature = "
        << average << " C" << std::endl;

    return average;
}