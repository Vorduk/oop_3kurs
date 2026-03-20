#pragma once
#include "IClimateManager.h"
#include "ControlStrategy.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

/**
 * @brief Менеджер климата, реализующий логику поддержания микроклимата
 *
 * Вычисление управляющих сигналов
 * делегируется объектам ControlStrategy, которые инкапсулируют логику
 * управления для каждого параметра (температура, влажность и т.д.).
 *
 * Каждая стратегия знает:
 * какой параметр контролирует
 * какие устройства использует
 * какой регулятор применяет (PID, On/Off и т.д.)
 *
 * ClimateManager собирает команды от всех стратегий и объединяет их,
 * разрешая конфликты (например, если два параметра пытаются управлять
 * одним устройством, выбирается максимальная мощность).
 */
class ClimateManager : public IClimateManager {
public:
    ClimateManager();

    /**
     * @brief Установить целевые значения параметров микроклимата
     * @param targets map "параметр -> целевое значение"
     */
    void setTargetParameters(const std::map<std::string, double>& targets) override;

    /**
     * @brief Рассчитать управляющие команды на основе текущих показаний
     * @param current_readings Текущие показания датчиков
     * @return map "тип устройства -> мощность (0-100)"
     */
    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& current_readings) override;

    /**
     * @brief Добавить стратегию управления (паттерн Делегирования)
     * @param strategy Умный указатель на стратегию
     */
    void addStrategy(std::shared_ptr<ControlStrategy> strategy);

    /**
     * @brief Установить регулятор для параметра (удобный метод-обёртка)
     * @param parameter Имя параметра ("temperature", "air_humidity" и т.д.)
     * @param regulator Умный указатель на регулятор
     *
     * Если стратегия для указанного параметра уже существует,
     * регулятор назначается ей. Если нет — создаётся новая стратегия.
     * Для параметра "temperature" автоматически создаётся стратегия
     * с двумя устройствами (heater и conditioner).
     */
    void setRegulator(const std::string& parameter, std::shared_ptr<IRegulator> regulator);

private:
    std::map<std::string, double> m_targets;                      ///< Целевые значения параметров
    std::vector<std::shared_ptr<ControlStrategy>> m_strategies;   ///< Список стратегий управления
};