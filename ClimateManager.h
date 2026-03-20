#pragma once
#include "IClimateManager.h"
#include <map>
#include <string>

class ClimateManager : public IClimateManager {
private:
    std::map<std::string, double> m_targets;  // параметр -> целевое значение
    std::map<std::string, std::string> m_mapping;  // параметр -> тип устройства

    double m_temp_hysteresis;
    double m_humidity_hysteresis;
    double m_soil_hysteresis;

public:
    ClimateManager();

    void setTargetParameters(const std::map<std::string, double>& targets) override;
    
    std::map<std::string, int> calculateCommands(const std::map<std::string, double>& current_readings) override;

    // Настройка маппинга (какой параметр каким устройством управляется)
    void setMapping(const std::string& parameter, const std::string& device_type);

    // Настройка гистерезиса
    void setHysteresis(double temp_hysteresis, double humidity_hysteresis, double soil_hysteresis);
};
