#include "ClimateAnalyzer.h"
#include <thread>
#include <chrono>
#include <sstream>

int ClimateAnalyzer::s_next_id = 1;

ClimateAnalyzer::ClimateAnalyzer()
    : m_id(s_next_id++)
    , m_analysis_count(0)
{
    std::cout << "[ClimateAnalyzer] Creating analyzer #" << m_id
        << " (heavy initialization)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "[ClimateAnalyzer] Analyzer #" << m_id << " ready." << std::endl;
}

ClimateAnalyzer::~ClimateAnalyzer() {
    std::cout << "[ClimateAnalyzer] Destroying analyzer #" << m_id << std::endl;
}

void ClimateAnalyzer::reset() {
    m_analysis_count = 0;
    std::cout << "[ClimateAnalyzer] Analyzer #" << m_id << " reset." << std::endl;
}

std::string ClimateAnalyzer::analyze(double temperature, double humidity, double soilMoisture) {
    ++m_analysis_count;
    std::ostringstream oss;
    oss << "Analyzer #" << m_id << " (call #" << m_analysis_count
        << "): T=" << temperature << "C, H=" << humidity
        << "%, Soil=" << soilMoisture << "%";

    if (temperature < 20.0) oss << "Need heating. ";
    else if (temperature > 28.0) oss << "Need cooling. ";
    else oss << "Temperature OK. ";

    if (humidity < 50.0) oss << "Need humidification. ";
    else if (humidity > 80.0) oss << "Need dehumidification. ";
    else oss << "Humidity OK. ";

    if (soilMoisture < 40.0) oss << "Need irrigation.";
    else if (soilMoisture > 70.0) oss << "Soil too wet.";
    else oss << "Soil moisture OK.";

    return oss.str();
}