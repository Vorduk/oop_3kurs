#pragma once
#include "ISystemState.h"
#include <map>
#include <string>

class NormalState : public ISystemState {
public:
    NormalState(double temp_min, double temp_max, double humidity_max);

    std::map<std::string, int> handle(
        const std::map<std::string, double>& readings,
        const std::map<std::string, double>& targets,
        ClimateManager* context) override;

    std::string getName() const override { return "NormalState"; }

private:
    double m_tempMin;
    double m_tempMax;
    double m_humidityMax;
};