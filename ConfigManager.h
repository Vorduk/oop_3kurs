#pragma once
#include "IConfigManager.h"
#include <map>
#include <string>

/**
 * @brief Базовый менеджер конфигурации для хранения целевых параметров микроклимата
 *
 * Хранит целевые значения параметров, к которым система управления должна стремиться.
 * Реализует интерфейс IConfigManager.
 *
 * Целевые параметры по умолчанию:
 * - температура: 22.0 °C
 * - влажность воздуха: 65.0 %
 * - влажность почвы: 45.0 %
 */
class ConfigManager : public IConfigManager {
public:
    /**
     * @brief Конструктор, инициализирует параметры значениями по умолчанию
     */
    ConfigManager();

    /**
     * @brief Получить целевое значение параметра
     * @param parameter Имя параметра ("temperature", "air_humidity", "soil_moisture")
     * @return Целевое значение параметра, 0.0 если параметр не найден
     */
    double getTargetParameter(const std::string& parameter) override;

    /**
     * @brief Установить целевое значение параметра
     * @param parameter Имя параметра
     * @param value Целевое значение (температура в °C, влажность в %)
     */
    void setTargetParameter(const std::string& parameter, double value) override;

    /**
     * @brief Получить все целевые параметры
     * @return map "имя параметра -> целевое значение"
     */
    std::map<std::string, double> getAllTargets() override;

protected:
    std::map<std::string, double> m_targets;  ///< Хранилище целевых параметров
};