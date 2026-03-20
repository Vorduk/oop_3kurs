#pragma once
#include "IClimateManager.h"
#include "ControlStrategy.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

class ClimateManager : public IClimateManager {
public:
    ClimateManager();

    void setTargetParameters(const std::map<std::string, double>& targets) override;
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& current_readings) override;

    // Паттерн Delegation: добавить стратегию управления
    void addStrategy(std::shared_ptr<ControlStrategy> strategy);

    // Установка регулятора через стратегию (удобный метод)
    void setRegulator(const std::string& parameter, std::shared_ptr<IRegulator> regulator);

private:
    std::map<std::string, double> m_targets;
    std::vector<std::shared_ptr<ControlStrategy>> m_strategies;
};