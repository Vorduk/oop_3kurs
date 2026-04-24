// main.cpp
#include <iostream>
#include <memory>
#include "DefaultGreenhouseBuilder.h"
#include "GreenhouseDirector.h"

int main() {
    std::cout << "=========================" << std::endl;
    std::cout << "Greenhouse Control System" << std::endl;
    std::cout << "=========================" << std::endl;

    // Строитель конфигурации по умолчанию
    std::shared_ptr<DefaultGreenhouseBuilder> builder = std::make_shared<DefaultGreenhouseBuilder>();

    // Директор с внедрённым строителем
    GreenhouseDirector director(builder);

    // Запуск сборки системы
    director.construct();

    // Получение готового движка
    std::shared_ptr<SimulationEngine> engine = builder->getResult();

    if (engine) {
        // Запуск главного цикла
        engine->start();
    }
    else {
        std::cerr << "Failed to build the greenhouse engine!" << std::endl;
        return 1;
    }

    return 0;
}