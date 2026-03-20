#pragma once
#include "ConfigManager.h"
#include <chrono>
#include <map>
#include <string>
#include <iostream>

/**
 * @brief Структура конфигурации периодической задачи
 *
 * Хранит настройки для устройства, которое должно работать по расписанию.
 * Используется в ExtendedConfigManager для хранения расписаний.
 */
struct ScheduleConfig {
    std::chrono::seconds interval;   ///< Интервал между выполнениями (в секундах)
    int powerLevel;                  ///< Уровень мощности (0-100%)
    bool enabled;                    ///< Активность расписания

    /**
     * @brief Конструктор по умолчанию (неактивное расписание)
     */
    ScheduleConfig() : interval(0), powerLevel(0), enabled(false) {}

    /**
     * @brief Конструктор с параметрами
     * @param seconds Интервал в секундах
     * @param power Уровень мощности (0-100%)
     */
    ScheduleConfig(int seconds, int power)
        : interval(seconds), powerLevel(power), enabled(true) {
    }
};

/**
 * @brief Расширенный менеджер конфигурации с поддержкой расписаний
 *
 * Наследует ConfigManager и добавляет функциональность хранения
 * и управления периодическими задачами (расписаниями).
 *
 * Использует паттерн Configuration: расписания хранятся в отдельном
 * контейнере и могут быть установлены, изменены или удалены.
 *
 * Пример использования:
 * @code
 * auto config = std::make_shared<ExtendedConfigManager>();
 * config->setSchedule("ventilation", 10, 50);  // вентиляция каждые 10 сек на 50%
 * config->setSchedule("lamp", 15, 70);         // лампы каждые 15 сек на 70%
 * @endcode
 */
class ExtendedConfigManager : public ConfigManager {
public:
    /**
     * @brief Конструктор, инициализирует параметры по умолчанию
     *
     * Устанавливает
     * температура: 22.0 °C
     * влажность воздуха: 65.0 %
     * влажность почвы: 45.0 %
     */
    ExtendedConfigManager();

    // Методы для работы с расписаниям

    /**
     * @brief Установить расписание для устройства
     * @param deviceType Тип устройства ("ventilation", "lamp", и т.д.)
     * @param intervalSeconds Интервал между выполнениями (в секундах)
     * @param powerLevel Уровень мощности (0-100%)
     *
     * Расписание сохраняется в контейнере m_schedules.
     * Если расписание для этого типа уже существовало, оно перезаписывается.
     *
     * @note Расписания используются SimulationEngine для создания фоновых таймеров
     */
    void setSchedule(const std::string& deviceType, int intervalSeconds, int powerLevel);

    /**
     * @brief Удалить расписание для устройства
     * @param deviceType Тип устройства
     */
    void removeSchedule(const std::string& deviceType);

    /**
     * @brief Получить расписание для устройства
     * @param deviceType Тип устройства
     * @return Конфигурация расписания или пустая, если расписание не найдено
     */
    ScheduleConfig getSchedule(const std::string& deviceType) const;

    /**
     * @brief Проверить наличие расписания для устройства
     * @param deviceType Тип устройства
     * @return true — расписание существует, false — нет
     */
    bool hasSchedule(const std::string& deviceType) const;

    /**
     * @brief Получить все расписания
     * @return Константная ссылка на map "тип устройства -> конфигурация"
     */
    const std::map<std::string, ScheduleConfig>& getAllSchedules() const;

private:
    std::map<std::string, ScheduleConfig> m_schedules;  ///< Хранилище расписаний
};