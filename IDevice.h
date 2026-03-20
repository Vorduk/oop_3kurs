#pragma once
#include <string>

/**
 * @brief Базовый интерфейс для всех исполнительных устройств
 *
 * Определяет минимальный набор методов, необходимых для управления любым устройством.
 */

class IDevice {
public:
    virtual ~IDevice() = default;

    // Общие методы для всех устройств

    /** @brief Включить устройство */
    virtual void turnOn() = 0;

    /** @brief Выключить устройство */
    virtual void turnOff() = 0;

    /**
     * @brief Проверить состояние устройства
     * @return true - устройство включено, false - выключено
     */
    virtual bool isOn() const = 0;

    /**
     * @brief Получить тип устройства
     * @return Строковый идентификатор типа (heater, conditioner, lamp и т.д.)
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Получить уникальный идентификатор устройства
     * @return Числовой ID устройства
     */
    virtual int getId() const = 0;
};

/**
 * @brief Интерфейс для устройств с плавной регулировкой мощности
 *
 * Используется для нагревателей, кондиционеров, ламп с регулировкой яркости.
 */
class IAdjustableDevice {
public:
    virtual ~IAdjustableDevice() = default;

    /**
     * @brief Установить уровень мощности
     * @param level Мощность в процентах (0-100)
     */
    virtual void setPower(int level) = 0;

    /**
     * @brief Получить текущий уровень мощности
     * @return Мощность в процентах (0-100)
     */
    virtual int getPower() const = 0;
};

/**
 * @brief Интерфейс для устройств с дискретными режимами работы
 *
 * Используется для увлажнителей, вентиляции, систем полива.
 */
class IModeSelectableDevice {
public:
    virtual ~IModeSelectableDevice() = default;

    /**
     * @brief Установить режим работы
     * @param mode Код режима (определяется в конкретном устройстве)
     */
    virtual void setMode(int mode) = 0;

    /**
     * @brief Получить текущий режим работы
     * @return Код текущего режима
     */
    virtual int getMode() const = 0;
};

