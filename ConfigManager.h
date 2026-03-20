#pragma once
#include "IConfigManager.h"
#include <map>
#include <string>

/**
 * @brief Менеджер конфигурации целевых параметров
 *
 * Хранит целевые значения температуры, влажности воздуха и почвы.
 */
class ConfigManager : public IConfigManager {
public:
    ConfigManager();

    double getTargetParameter(const std::string& parameter) override;
    void setTargetParameter(const std::string& parameter, double value) override;
    std::map<std::string, double> getAllTargets() override;

private:
    std::map<std::string, double> m_targets;
};