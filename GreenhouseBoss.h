#pragma once
#include "SystemMemento.h"
#include "SimulationModel.h"
#include "ClimateManager.h"
#include <memory>

// Класс, создающий и восстанавливающий снимки системы.
class GreenhouseBoss {
public:
    GreenhouseBoss(std::shared_ptr<SimulationModel> model,
        std::shared_ptr<ClimateManager> climateManager);

    // Создать снимок текущего состояния.
    SystemMemento createMemento() const;

    // Восстановить состояние из снимка.
    void restoreFromMemento(const SystemMemento& memento);

private:
    std::shared_ptr<SimulationModel> m_model;
    std::shared_ptr<ClimateManager> m_climateManager;
};