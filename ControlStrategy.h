#pragma once
#include "IRegulator.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class ControlStrategy {
public:
    ControlStrategy(const std::string& paramName,
        const std::vector<std::string>& devices,
        std::shared_ptr<IRegulator> regulator = nullptr);

    std::map<std::string, int> calculate(
        const std::map<std::string, double>& currentReadings,
        const std::map<std::string, double>& targets) const;

    const std::string& getParamName() const { return m_paramName; }
    void setRegulator(std::shared_ptr<IRegulator> regulator) { m_regulator = regulator; }
    void setDevices(const std::vector<std::string>& devices) { m_devices = devices; }

private:
    void logAction(double current, double target, double signal,
        const std::string& device) const;

    std::string m_paramName;
    std::vector<std::string> m_devices;
    std::shared_ptr<IRegulator> m_regulator;
};