#include <iostream>
#include <memory>
#include "SignalProcessor.h"
#include "TemperatureHandler.h"
#include "HumidityHandler.h"
#include "SoilMoistureHandler.h"
#include "DefaultHandler.h"

int main() {

    // Полная цепочка через SignalProcessor
    SignalProcessor processor;
    std::cout << "\nFull chain\n";
    processor.process_signal("temperature", 15.0);
    processor.process_signal("air_humidity", 40.0);
    processor.process_signal("soil_moisture", 25.0);

    // Разрыв цепочки: цепочка без HumidityHandler
    std::cout << "\nBroken chain (without HumidityHandler)\n";
    auto temp = std::make_shared<TemperatureHandler>();
    auto soil = std::make_shared<SoilMoistureHandler>();
    auto def = std::make_shared<DefaultHandler>();

    temp->set_next(soil);  // пропуск HumidityHandler
    soil->set_next(def);

    // Отправка сигнала air_humidity – он не обрабатывается,
    // проходит через TemperatureHandler и SoilMoistureHandler до DefaultHandler
    std::string result = temp->handle("air_humidity", 40.0);
    std::cout << "Result: " << result << std::endl;

    // Прохождение до конца цепочки: неизвестный сигнал
    std::cout << "\nUnknown signal\n";
    processor.process_signal("light_intensity", 500.0);

    return 0;
}