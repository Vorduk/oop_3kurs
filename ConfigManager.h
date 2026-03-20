#pragma once
#include "IConfigManager.h"
#include <map>
#include <string>

class ConfigManager : public IConfigManager {
public:
    ConfigManager();

    double getTargetParameter(const std::string& parameter) override;
    void setTargetParameter(const std::string& parameter, double value) override;
    std::map<std::string, double> getAllTargets() override;

protected:
    std::map<std::string, double> m_targets;
};