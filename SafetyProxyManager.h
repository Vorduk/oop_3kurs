#pragma once
#include "IClimateManager.h"
#include "IIOManager.h"
#include <memory>
#include <map>
#include <string>
#include "TemperatureEmergencyStrategy.h"

/**
 * @brief Прокси безопасности – реализует тот же интерфейс, что и реальный менеджео
 *
 * Все настройки (стратегия аварий, пороги) передаются через конструктор.
 * Дополнительные методы (isEmergencyMode) – приватные, для внутреннего использования
 */
class SafetyProxyManager : public IClimateManager {
public:
    /**
     * @param realManager Реальный менеджер климата (оборачиваемый объект)
     * @param ioManager Менеджер ввода/вывода (для чтения датчиков при фильтрации)
     * @param strategy Стратегия аварийного реагирования (может быть nullptr)
     */
    SafetyProxyManager(std::shared_ptr<IClimateManager> realManager,
        std::shared_ptr<IIOManager> ioManager,
        std::shared_ptr<IEmergencyStrategy> strategy = nullptr);

    void setTargetParameters(const std::map<std::string, double>& targets) override;
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) override;

private:
    // Приватные методы – только для внутренней логики прокси
    bool isEmergencyMode() const;                  
    std::string getEmergencyReason() const;        // для логов
    void logEmergency(const std::string& reason);  // логирование

    std::shared_ptr<IClimateManager> m_realManager;
    std::shared_ptr<IIOManager> m_ioManager;
    std::shared_ptr<IEmergencyStrategy> m_emergencyStrategy;
    bool m_emergencyMode;
    std::string m_emergencyReason;
};