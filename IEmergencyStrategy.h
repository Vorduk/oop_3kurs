#include <memory>
#include <iostream>
#include <map>
#pragma once
/**
 * @brief Интерфейс стратегии аварийного реагирования
 *
 * Паттерн Delegation: SafetyProxyManager делегирует логику аварийных команд
 * специализированным стратегиям. Это позволяет легко добавлять новые типы
 * аварий (пожар, наводнение, поломка датчиков) без изменения прокси.
 */
class IEmergencyStrategy {
public:
    virtual ~IEmergencyStrategy() = default;

    /**
     * @brief Получить команды для аварийной ситуации
     * @return map устройство -> мощность/режим
     */
    virtual std::map<std::string, int> getEmergencyCommands() const = 0;

    /**
     * @brief Проверить, является ли ситуация аварийной
     * @param readings Текущие показания датчиков
     * @param reason [out] Причина аварии (для логирования)
     * @return true - авария, false - штатный режим
     */
    virtual bool isEmergency(const std::map<std::string, double>& readings,
        std::string& reason) const = 0;

    /**
     * @brief Отфильтровать опасные команды в штатном режиме
     * @param commands Команды от реального менеджера
     * @param readings Текущие показания
     * @return Отфильтрованные безопасные команды
     */
    virtual std::map<std::string, int> filterDangerousCommands(
        const std::map<std::string, int>& commands,
        const std::map<std::string, double>& readings) const {
        return commands; // По умолчанию без фильтрации
    }

    virtual std::string getName() const = 0;
};