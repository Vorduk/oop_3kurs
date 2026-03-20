#pragma once
#include "BaseDevice.h"

/**
 * @brief Лампы освещения для обеспечения светового дня
 *
 * Поддерживает регулировку яркости (0-100%) и выбор спектра (белый/теплый/УФ).
 * Реализует интерфейсы IDevice, IAdjustableDevice и IModeSelectableDevice.
 */
class Lamp : public BaseDevice, public IAdjustableDevice, public IModeSelectableDevice {
public:
    Lamp();
    ~Lamp() override = default;

    // IDevice implementation

    /** @brief Вкл лампу */
    void turnOn() override;

    /** @brief Выкл лампу */
    void turnOff() override;

    /** @brief Включена ли лампа */
    bool isOn() const override;


    // IAdjustableDevice implementation

    /** @brief Изменить мощность лампы */
    void setPower(int level) override;

    /** @brief Получить мощность лампы */
    int getPower() const override;


    // IModeSelectableDevice implementation

    /** @brief Установить режим освещения лампы */
    void setMode(int mode) override;

    /** @brief Получить режим освещения лампы */
    int getMode() const override;

    /** @brief Режимы освещения (спектр) */
    static constexpr int MODE_WHITE = 1;    ///< Белый свет (дневной)
    static constexpr int MODE_WARM = 2;     ///< Теплый свет (вечерний)
    static constexpr int MODE_UF = 3;       ///< Ультрафиолетовый (для роста)

private:
    bool m_isOn;           ///< Состояние устройства (включен/выключен)
    int m_powerLevel;      ///< Уровень яркости (0-100%)
    int m_mode;            ///< Режим спектра (1-3)
};

