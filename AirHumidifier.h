#pragma once
#include "BaseDevice.h"

/**
 * @brief Увлажнитель для повышения влажности воздуха
 *
 * Поддерживает дискретные режимы работы (низкий/средний/высокий).
 * Реализует интерфейсы IDevice и IModeSelectableDevice.
 */
class AirHumidifier : public BaseDevice, public IModeSelectableDevice {
public:
    AirHumidifier();
    ~AirHumidifier() override = default;

    // Методы IDevice

    /** @brief Вкл увлажнитель воздуха */
    void turnOn() override;

    /** @brief Выкл увлажнитель воздуха */
    void turnOff() override;

    /** @brief Включён ли увлажнитель воздуха */
    bool isOn() const override;


    // Методы IModeSelectableDevice

    /** @brief Установить режим работы увлажнителя воздуха */
    void setMode(int mode) override;

    /** @brief Получить режим увлажнителя воздуха */
    int getMode() const override;

    /** @brief Режимы увлажнителя воздуха */
    static constexpr int MODE_LOWEST = 1;
    static constexpr int MODE_LOW = 2;
    static constexpr int MODE_MEDIUM = 3;
    static constexpr int MODE_HIGH = 4;

private:
    bool m_isOn;
    int m_mode; // 1 - слабый, 2 - средний, 3 - сильный

};
