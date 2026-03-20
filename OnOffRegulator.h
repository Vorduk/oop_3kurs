#pragma once
#include "IRegulator.h"
#include <cmath>

/**
 * @brief Регулятор типа "включено/выключено" (On/Off) с гистерезисом
 *
 * Реализует алгоритм позиционного регулирования, при котором
 * управляющий сигнал может принимать только два значения: 0% (выключено)
 * или 100% (включено). Гистерезис предотвращает постоянное переключение
 * устройства вблизи целевого значения.
 */
class OnOffRegulator : public IRegulator {
public:
    /**
     * @brief Конструктор регулятора
     * @param hysteresis Ширина гистерезиса.
     *        Например, при hysteresis = 2.0, устройство не будет переключаться
     *        пока значение находится в диапазоне [target - 2.0, target + 2.0]
     */
    OnOffRegulator(double hysteresis = 1.0)
        : m_hysteresis(hysteresis), m_lastState(false) {
    }

    /**
     * @brief Рассчитать управляющий сигнал на основе текущего и целевого значения
     *
     * Алгоритм работы:
     * Если текущее значение ниже (target - hysteresis): вкл устройство (100%)
     * Если текущее значение выше (target + hysteresis): выкл устройство (0%)
     * В зоне гистерезиса [target - hysteresis, target + hysteresis]:
     * сохраняется предыдущее состояние (избегание частых переключений)
     *
     * @param current Текущее значение параметра (температура, влажность и т.д.)
     * @param target Целевое значение параметра
     * @return 100.0 - включить устройство, 0.0 - выключить устройство
     */
    double calculate(double current, double target) override;

    /**
     * @brief Получить название регулятора
     * @return Строка "OnOffRegulator"
     */
    std::string getName() const override;

    /**
     * @brief Установить ширину гистерезиса
     * @param hysteresis Новая ширина гистерезиса
     */
    void setHysteresis(double hysteresis);

private:
    double m_hysteresis;    ///< Ширина гистерезиса (мертвая зона) в единицах параметра
    bool m_lastState;       ///< Последнее состояние (true - включено, false - выключено)
};