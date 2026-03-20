#pragma once
#include "IClimateManager.h"
#include "IIOManager.h"
#include <memory>
#include <iostream>
#include <map>
#include "TemperatureEmergencyStrategy.h"

/**
 * @brief Паттерн Proxy: заместитель для IClimateManager с проверкой безопасности
 *
 * SafetyProxyManager оборачивает реальный менеджер климата, добавляя
 * дополнительные проверки безопасности. Перед передачей команд реальному
 * менеджеру прокси проверяет, не находится ли система в аварийном состоянии.
 *
 * Логика аварийного реагирования делегируется
 * стратегии IEmergencyStrategy, что делает прокси легко расширяемым.
 * Можно добавлять новые стратегии аварий
 * (пожар, наводнение, поломка датчиков) без изменения кода прокси.
 *
 * @note Реальный менеджер может быть любым объектом, реализующим IClimateManager
 * @see IEmergencyStrategy
 * @see TemperatureEmergencyStrategy
 */
class SafetyProxyManager : public IClimateManager {
public:
    /**
     * @brief Конструктор прокси безопасности
     * @param realManager Реальный менеджер климата (оборачиваемый объект)
     * @param ioManager Менеджер ввода/вывода (для доступа к датчикам)
     */
    SafetyProxyManager(std::shared_ptr<IClimateManager> realManager,
        std::shared_ptr<IIOManager> ioManager);

    /**
     * @brief Установить стратегию аварийного реагирования
     *
     * Прокси делегирует проверки и команды стратегии.
     *
     * @param strategy Стратегия аварийного реагирования
     *
     * @see IEmergencyStrategy
     */
    void setEmergencyStrategy(std::shared_ptr<IEmergencyStrategy> strategy);

    /**
     * @brief Установить критические пороги (метод для обратной совместимости)
     *
     * Создаёт стратегию TemperatureEmergencyStrategy по умолчанию,
     * если она ещё не установлена. Позволяет использовать прокси
     * без явного создания стратегии.
     *
     * @param tempMin Минимальная допустимая температура (°C)
     * @param tempMax Максимальная допустимая температура (°C)
     * @param humidityMax Максимальная допустимая влажность (%)
     */
    void setCriticalThresholds(double tempMin, double tempMax, double humidityMax);

    /**
     * @brief Установить целевые параметры микроклимата
     *
     * Делегирует вызов реальному менеджеру. Прокси не изменяет
     * целевые параметры, только передаёт их дальше.
     *
     * @param targets Ассоциативный массив "параметр -> целевое значение"
     */
    void setTargetParameters(const std::map<std::string, double>& targets) override;

    /**
     * @brief Рассчитать управляющие команды с проверкой безопасности
     *
     * Проверить наличие аварийной ситуации через стратегию
     * Если авария — вернуть аварийные команды (безопасный режим)
     * Если штатная ситуация — запросить команды у реального менеджера
     * Отфильтровать опасные команды через стратегию
     * Вернуть безопасные команды
     *
     * @param currentReadings Текущие показания датчиков
     * @return Безопасные команды для исполнительных устройств
     */
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) override;

    /**
     * @brief Проверить, находится ли система в аварийном режиме
     * @return true — аварийный режим, false — штатный режим
     */
    bool isEmergencyMode() const;

    /**
     * @brief Получить причину последней аварийной ситуации
     * @return Строка с описанием причины (для логирования)
     */
    std::string getEmergencyReason() const;

private:
    std::shared_ptr<IClimateManager> m_realManager;     ///< Реальный менеджер (оборачиваемый объект)
    std::shared_ptr<IIOManager> m_ioManager;            ///< Менеджер ввода/вывода (для доступа к датчикам)
    std::shared_ptr<IEmergencyStrategy> m_emergencyStrategy;    ///< Стратегия аварийного реагирования

    bool m_emergencyMode;           ///< Флаг аварийного режима (true — авария)
    std::string m_emergencyReason;  ///< Причина последней аварии
};