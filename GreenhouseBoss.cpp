#include "GreenhouseBoss.h"
#include <iostream>

GreenhouseBoss::GreenhouseBoss(std::shared_ptr<SimulationModel> model,
    std::shared_ptr<ClimateManager> climateManager)
    : m_model(model), m_climateManager(climateManager) {
}

SystemMemento GreenhouseBoss::createMemento() const {
    double temp = m_model->getTemperature();
    double humidity = m_model->getAirHumidity();
    double soil = m_model->getSoilMoisture();
    std::map<std::string, double> targets = m_climateManager->getTargets();
    // Команды устройств пока не сохраняются (можно расширить).
    std::map<std::string, int> deviceCommands;

    std::cout << "[GreenhouseOriginator] Creating snapshot: T=" << temp << " C"
        << ", H=" << humidity << " %"
        << ", S=" << soil << " %" << std::endl;
    return SystemMemento(temp, humidity, soil, targets, deviceCommands);
}

void GreenhouseBoss::restoreFromMemento(const SystemMemento& memento) {
    // Восстановление параметров модели
    m_model->setTemperature(memento.getTemperature());
    m_model->setAirHumidity(memento.getAirHumidity());
    m_model->setSoilMoisture(memento.getSoilMoisture());
    // Восстановление целевых уставок
    m_climateManager->setTargetParameters(memento.getTargets());

    std::cout << "[GreenhouseOriginator] Restored snapshot: T=" << memento.getTemperature()
        << " C, H=" << memento.getAirHumidity()
        << " %, S=" << memento.getSoilMoisture() << " %" << std::endl;
}