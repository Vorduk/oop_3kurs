#pragma once
#include "IConfigManager.h"
#include <map>
#include <string>

// Устанавливает целевые параметры которые должна поддерживать система
class ConfigManager : public IConfigManager {
private:
    std::map<std::string, double> m_targets;

public:
    ConfigManager();

    double getTargetParameter(const std::string& parameter) override;
    void setTargetParameter(const std::string& parameter, double value) override;
    std::map<std::string, double> getAllTargets() override;
};