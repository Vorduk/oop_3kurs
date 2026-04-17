#pragma once
#include "IInformationExpert.h"
#include "IAggregate.h"
#include <string>
#include <memory>

/**
 * @brief Эксперт для расчёта суммарной энергии, потреблённой устройством
 */
class TotalEnergyExpert : public IInformationExpert {
public:
    /**
     * @brief Конструктор
     * @param history Источник данных — история команд (агрегат)
     * @param deviceType Тип устройства для анализа ("heater", "conditioner", ...)
     */
    TotalEnergyExpert(std::shared_ptr<IAggregate> history, const std::string& deviceType);

    /**
     * @brief Вычислить суммарную энергию
     * @return Сумма значений мощности (в каких-то условных единицах)
     */
    double getValue() override;

private:
    std::shared_ptr<IAggregate> m_history;
    std::string m_deviceType;
};