#pragma once
#include "IInformationExpert.h"
#include "IIOManager.h"
#include <memory>

/**
 * @brief Эксперт для расчёта средней температуры по всем датчикам
 */
class AverageTemperatureExpert : public IInformationExpert {
public:
    /**
     * @brief Конструктор
     * @param ioManager Источник данных
     */
    AverageTemperatureExpert(std::shared_ptr<IIOManager> io_manager);

    /**
     * @brief Вычислить среднюю температуру
     * @return Средняя температура в градусах Цельсия
     */
    double getValue() override;

private:
    std::shared_ptr<IIOManager> m_io_manager;
};