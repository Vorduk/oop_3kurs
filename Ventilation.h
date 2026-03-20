#pragma once
#include "BaseDevice.h"

/**
 * @brief Вентиляция для воздухообмена в теплице
 *
 * Поддерживает дискретные режимы работы (низкий/средний/высокий).
 * Реализует интерфейсы IDevice и IModeSelectableDevice.
 */
class Ventilation : public BaseDevice, public IModeSelectableDevice {
public:
    Ventilation();
    ~Ventilation() override = default;

    // Методы IDevice

    /** @brief Вкл вентиляцию */
    void turnOn() override;

    /** @brief Выкл вентиляцию */
    void turnOff() override;

    /** @brief Включена ли вентиляция */
    bool isOn() const override;


    // Методы IModeSelectableDevice

    /** @brief Установить режим работы вентиляции */
    void setMode(int mode) override;

    /** @brief Получить режим вентиляции */
    int getMode() const override;

    /** @brief Режимы работы вентиляции */
    static constexpr int MODE_LOW = 1;      ///< Слабый режим
    static constexpr int MODE_MEDIUM = 2;   ///< Средний режим
    static constexpr int MODE_HIGH = 3;     ///< Сильный режим

private:
    bool m_isOn;    ///< Состояние устройства (включен/выключен)
    int m_mode;     ///< Текущий режим работы (1-3)
};

