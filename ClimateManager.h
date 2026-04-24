#pragma once
#include "IClimateManager.h"
#include "ControlStrategy.h"
#include "ISystemState.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

class ClimateManager : public IClimateManager {
public:
    ClimateManager();
    ~ClimateManager();

    // Установить целевые параметры (делегируется стратегиям).
    void setTargetParameters(const std::map<std::string, double>& targets) override;

    // Основной метод расчёта команд: делегирует текущему состоянию.
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) override;

    // Добавить стратегию управления.
    void addStrategy(std::shared_ptr<ControlStrategy> strategy);

    // Назначить регулятор для параметра.
    void setRegulator(const std::string& parameter, std::shared_ptr<IRegulator> regulator);

    // Сменить текущее состояние (используется состояниями при переходе).
    void setState(ISystemState* newState);

    // Вычислить команды, используя заложенные стратегии (без учёта состояний).
    // Вызывается из NormalState и аварийных состояний после возврата к норме.
    std::map<std::string, int> computeStandardCommands(
        const std::map<std::string, double>& currentReadings,
        const std::map<std::string, double>& targets) const;

private:
    std::map<std::string, double> m_targets;
    std::vector<std::shared_ptr<ControlStrategy>> m_strategies;
    ISystemState* m_currentState;  // владение состоянием, может быть сырым указателем (удаляем при смене)
};