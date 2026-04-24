#pragma once
#include "ISystemState.h"
#include <map>
#include <string>

class EmergencyCoolingState : public ISystemState {
public:
    EmergencyCoolingState();

    std::map<std::string, int> handle(
        const std::map<std::string, double>& readings,
        const std::map<std::string, double>& targets,
        ClimateManager* context) override;

    std::string getName() const override { return "EmergencyCoolingState"; }
};