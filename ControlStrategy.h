#pragma once
#include "IRegulator.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

/**
 * @brief Стратегия управления одним параметром микроклимата
 *
 * Инкапсулирует логику управления для конкретного параметра (температура,
 * влажность воздуха, влажность почвы).
 *
 * Класс знает
 * какой параметр контролирует (m_paramName)
 * какие устройства использует (m_devices) — одно или два
 * какой алгоритм регулирования применяет (m_regulator)
 *
 * Для одного устройства: положительный сигнал -> устройство включается
 * Для двух устройств (температура): положительный сигнал -> нагреватель,
 * отрицательный -> кондиционер
 * Если устройств нет, то только логирование
 */
class ControlStrategy {
public:
    /**
     * @brief Конструктор стратегии управления
     * @param paramName Имя параметра ("temperature", "air_humidity", "soil_moisture")
     * @param devices Список устройств, управляющих параметром
     * @param regulator Регулятор для вычисления управляющего сигнала (может быть nullptr)
     *
     * @note Если regulator == nullptr, стратегия не будет выдавать команды,
     * но продолжит логировать текущие значения
     */
    ControlStrategy(const std::string& paramName,
        const std::vector<std::string>& devices,
        std::shared_ptr<IRegulator> regulator = nullptr);

    /**
     * @brief Вычислить управляющие команды на основе текущих и целевых значений
     * @param currentReadings Текущие показания всех датчиков
     * @param targets Целевые значения параметров
     * @return Ассоциативный массив "устройство -> мощность (0-100)"
     *
     * Алгоритм работы:
     * Найти текущее и целевое значение для управляемого параметра
     * Если параметр не найден — вернуть пустые команды
     * Делегировать расчёт сигнала регулятору
     * Преобразовать сигнал в команды для устройств:
     *      Одно устройство: сигнал > 0 -> команда, иначе 0
     *      Два устройства: сигнал > 0 -> первое устройство,
     *                      сигнал < 0 -> второе устройство,
     *                      сигнал = 0 -> оба выключены
     * 5. Записать результат в лог
     */
    std::map<std::string, int> calculate(
        const std::map<std::string, double>& currentReadings,
        const std::map<std::string, double>& targets) const;

    /// @brief Получить имя управляемого параметра
    const std::string& getParamName() const { return m_paramName; }

    /// @brief Установить регулятор (паттерн Делегирования)
    void setRegulator(std::shared_ptr<IRegulator> regulator) { m_regulator = regulator; }

    /// @brief Установить список устройств для управления параметром
    void setDevices(const std::vector<std::string>& devices) { m_devices = devices; }

private:
    /**
     * @brief Записать действие в лог
     * @param current Текущее значение параметра
     * @param target Целевое значение параметра
     * @param signal Управляющий сигнал (0-100 или 0 для оптимального состояния)
     * @param device Название устройства или "optimal"/"no_device"
     */
    void logAction(double current, double target, double signal,
        const std::string& device) const;

    std::string m_paramName;                    ///< Имя управляемого параметра
    std::vector<std::string> m_devices;         ///< Устройства для управления
    std::shared_ptr<IRegulator> m_regulator;    ///< Регулятор (алгоритм управления)
};