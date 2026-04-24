#include "ClimateManager.h"
#include "NormalState.h"
#include <iostream>

ClimateManager::ClimateManager()
    : m_currentState(nullptr) {
    // Начальное состояние — нормальное с порогами
    m_currentState = new NormalState(5.0, 40.0, 90.0);
    std::cout << "[ClimateManager] Created with initial state: NormalState\n";
}

ClimateManager::~ClimateManager() {
    delete m_currentState;
}

void ClimateManager::setTargetParameters(const std::map<std::string, double>& targets) {
    m_targets = targets;
    std::cout << "[ClimateManager] Set initial params:\n";
    // Перебор без auto: используем константный итератор
    for (std::map<std::string, double>::const_iterator it = targets.begin();
        it != targets.end(); ++it) {
        std::cout << "  " << it->first << " = " << it->second << std::endl;
    }
}

void ClimateManager::addStrategy(std::shared_ptr<ControlStrategy> strategy) {
    if (strategy) {
        m_strategies.push_back(strategy);
        std::cout << "[ClimateManager] Added strategy for " << strategy->getParamName() << std::endl;
    }
}

void ClimateManager::setRegulator(const std::string& parameter,
    std::shared_ptr<IRegulator> regulator) {
    // Поиск существующей стратегии
    for (std::vector<std::shared_ptr<ControlStrategy>>::const_iterator it = m_strategies.begin();
        it != m_strategies.end(); ++it) {
        if ((*it)->getParamName() == parameter) {
            (*it)->setRegulator(regulator);
            return;
        }
    }
    // Если не найдена, создаётся новая с устройствами по имени параметра
    std::vector<std::string> devices;
    if (parameter == "temperature") {
        devices.push_back("heater");
        devices.push_back("conditioner");
    }
    else if (parameter == "air_humidity") {
        devices.push_back("air_humidifier");
    }
    else if (parameter == "soil_moisture") {
        devices.push_back("irrigation");
    }
    std::shared_ptr<ControlStrategy> strategy = std::make_shared<ControlStrategy>(
        parameter, devices, regulator);
    m_strategies.push_back(strategy);
    std::cout << "[ClimateManager] Added strategy for " << parameter << "\n";
}

std::map<std::string, int> ClimateManager::calculateCommands(
    const std::map<std::string, double>& currentReadings) {
    // Делегирование текущему состоянию
    if (m_currentState) {
        return m_currentState->handle(currentReadings, m_targets, this);
    }
    std::map<std::string, int> empty;
    return empty;
}

void ClimateManager::setState(ISystemState* newState) {
    if (m_currentState) {
        std::cout << "[ClimateManager] State changed: "
            << m_currentState->getName() << " -> " << newState->getName() << std::endl;
        delete m_currentState;
    }
    m_currentState = newState;
}

std::map<std::string, int> ClimateManager::computeStandardCommands(
    const std::map<std::string, double>& currentReadings,
    const std::map<std::string, double>& targets) const {
    std::map<std::string, int> allCommands;
    for (std::vector<std::shared_ptr<ControlStrategy>>::const_iterator stratIt = m_strategies.begin();
        stratIt != m_strategies.end(); ++stratIt) {
        const std::shared_ptr<ControlStrategy>& strategy = *stratIt;
        std::map<std::string, int> commands = strategy->calculate(currentReadings, targets);
        for (std::map<std::string, int>::const_iterator cmdIt = commands.begin();
            cmdIt != commands.end(); ++cmdIt) {
            const std::string& dev = cmdIt->first;
            int power = cmdIt->second;
            std::map<std::string, int>::iterator it = allCommands.find(dev);
            if (it != allCommands.end()) {
                if (power > it->second) {
                    it->second = power;
                }
            }
            else {
                allCommands[dev] = power;
            }
        }
    }
    return allCommands;
}