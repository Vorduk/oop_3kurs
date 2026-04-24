#pragma once
#include <map>
#include <string>

// Класс-хранитель, хранящий снимок состояния системы.
class SystemMemento {
public:
    SystemMemento();
    SystemMemento(double temp, double humidity, double soil,
        const std::map<std::string, double>& targets,
        const std::map<std::string, int>& deviceCommands);

    double getTemperature() const;
    double getAirHumidity() const;
    double getSoilMoisture() const;
    const std::map<std::string, double>& getTargets() const;
    const std::map<std::string, int>& getDeviceCommands() const;

private:
    double m_temperature;
    double m_airHumidity;
    double m_soilMoisture;
    std::map<std::string, double> m_targets;
    std::map<std::string, int> m_deviceCommands;
};