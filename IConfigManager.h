#pragma once
#include <map>
#include <string>

// Интерфайс менеджера целевых конфигураций - к каким параметрам стремится прийти теплица
class IConfigManager {
public:
    virtual ~IConfigManager() = default;

    virtual double getTargetParameter(const std::string& parameter) = 0;
    virtual void setTargetParameter(const std::string& parameter, double value) = 0;
    virtual std::map<std::string, double> getAllTargets() = 0;
};