#pragma once
#include "BaseDevice.h"
#include "OldHeater.h"
#include "IDeviceVisitor.h"
#include <memory>

class OldHeaterAdapter : public BaseDevice, public IAdjustableDevice {
public:
    OldHeaterAdapter(std::shared_ptr<OldHeater> legacy);
    ~OldHeaterAdapter();

    // Методы IDevice

   /** @brief Вкл нагреватель */
    void turnOn() override;

    /** @brief Выкл нагреватель */
    void turnOff() override;

    /** @brief Включён ли нагреватель */
    bool isOn() const override;


    // Методы IAdjustableDevice

    /** @brief Изменить мощность нагревателя */
    void setPower(int level) override;

    /** @brief Получить текущую мощность нагревателя */
    int getPower() const override;

    void accept(IDeviceVisitor* visitor) override {
        visitor->visit(this);
    }

private:
    std::shared_ptr<OldHeater> m_legacy;    ///< Адаптируемый объект
    bool m_is_on;           ///< Состояние устройства (включен/выключен)
    int m_powerLevel;       ///< Уровень мощности в процентах (0-100)
};

