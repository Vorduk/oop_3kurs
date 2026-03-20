#pragma once
#include "IClimateManager.h"
#include "IRegulator.h"
#include <map>
#include <string>
#include <memory>

class ClimateManager : public IClimateManager {
private:
    std::map<std::string, double> m_targets;
    std::map<std::string, std::string> m_mapping;

    // Паттерн Delegation: для каждого параметра свой регулятор
    std::map<std::string, std::shared_ptr<IRegulator>> m_regulators;

    // Настройки гистерезиса (используются если нет регулятора)
    double m_temp_hysteresis;
    double m_humidity_hysteresis;
    double m_soil_hysteresis;

public:
    ClimateManager();

    void setTargetParameters(const std::map<std::string, double>& targets) override;
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& current_readings) override;

    // Настройка маппинга параметр -> устройство
    void setMapping(const std::string& parameter, const std::string& device_type);

    // Паттерн Delegation: назначить регулятор для параметра
    void setRegulator(const std::string& parameter, std::shared_ptr<IRegulator> regulator);

    // Для обратной совместимости (гистерезис для On/Off)
    void setHysteresis(double temp_hysteresis, double humidity_hysteresis, double soil_hysteresis);

private:
    double calculateControlSignal(const std::string& parameter, double current, double target);
};