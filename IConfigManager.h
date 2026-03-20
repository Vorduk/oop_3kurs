#pragma once
#include <map>
#include <string>

/**
 * @brief Интерфейс менеджера целевых конфигураций
 *
 * Задает целевые значения параметров, к которым система управления должна
 * приводить теплицу: температуру, влажность воздуха, влажность почвы.
 */
class IConfigManager {
public:
    virtual ~IConfigManager() = default;

    /**
     * @brief Получить целевое значение параметра
     * @param parameter Имя параметра (например "temperature", "air_humidity", "soil_moisture")
     * @return Целевое значение параметра
     */
    virtual double getTargetParameter(const std::string& parameter) = 0;

    /**
     * @brief Установить целевое значение параметра
     * @param parameter Имя параметра
     * @param value Целевое значение
     */
    virtual void setTargetParameter(const std::string& parameter, double value) = 0;

    /**
     * @brief Получить все целевые параметры
     * @return map "имя параметра -> целевое значение"
     */
    virtual std::map<std::string, double> getAllTargets() = 0;
};