#pragma once
#include "BaseDevice.h"

/**
 * @brief Система капельного полива для увлажнения почвы
 *
 * Поддерживает дискретные режимы работы (капельный/разбрызгивание).
 * Реализует интерфейсы IDevice и IModeSelectableDevice.
 */
class Irrigation : public BaseDevice, public IModeSelectableDevice {
public:
    Irrigation();
    ~Irrigation() override = default;

    //  Методы IDevice

    /** @brief Вкл систему полива */
    void turnOn() override;

    /** @brief Выкл систему полива */
    void turnOff() override;

    /** @brief Включён ли система полива */
    bool isOn() const override;


    // Методы IModeSelectableDevice

    /** @brief Установить режим полива */
    void setMode(int mode) override;

    /** @brief Получить режим полива */
    int getMode() const override;

    /** @brief Режимы полива */
    static constexpr int MODE_DRIP = 1;         ///< Капельный полив
    static constexpr int MODE_SPRINKLER = 2;    ///< Разбрызгивание

private:
    bool m_isOn;    ///< Состояние устройства (включен/выключен)
    int m_mode;     ///< Текущий режим полива (1-2)
};