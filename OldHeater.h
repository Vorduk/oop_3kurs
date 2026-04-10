#pragma once

class OldHeater {
public:
    OldHeater();

    /** @brief Вкл нагреватель */
    void powerOn();

    /** @brief Выкл нагреватель */
    void powerOff();

    /** @brief Изменить мощность нагревателя */
    void setHeatLevel(int level);

    /** @brief Получить текущую мощность нагревателя */
    int getHeatLevel();
private:
    int m_heat_level;   ///< Уровень мощности в процентах (0-10)
    bool m_power;       ///< Состояние устройства (включен/выключен)
};

