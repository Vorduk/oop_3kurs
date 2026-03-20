#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <string>

// Базовый класс сенсора, от него наследуются все остальные сенсоры
class BaseSensor : public ISensor {

protected:
    const IDataProvider* m_data_provider; // Модель откуда берутся данные
    std::string m_type; // Тип датчика
    int m_id;  // Уникальный числовой идентификатор
    static int s_next_id;  // Статический счетчик для генерации ID

public:
    BaseSensor(const IDataProvider* data_provider, const std::string& type);

    std::string getType() const override;
    int getId() const override;

    virtual ~BaseSensor() = default;
};

