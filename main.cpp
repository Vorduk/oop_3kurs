#include "GreenhouseSystemFacade.h"
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System (with Facade)" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    GreenhouseSystemFacade facade;

    facade.setTargetParameter("temperature", 23.0);
    facade.setTargetParameter("air_humidity", 65.0);
    facade.setTargetParameter("soil_moisture", 50.0);

    facade.setSchedule("ventilation", 10, 50);
    facade.setSchedule("lamp", 15, 70);

    std::cout << "\n========================================" << std::endl;
    std::cout << "System configured. Starting main loop..." << std::endl;
    std::cout << "========================================" << std::endl;

    facade.start();

    return 0;
}