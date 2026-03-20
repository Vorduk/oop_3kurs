#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <string>

class BaseSensor : public ISensor {

protected:
    const IDataProvider* m_dataProvider;
    std::string m_type; // Тип датчика
    int m_id;  // Уникальный числовой идентификатор
    static int s_next_id;  // Статический счетчик для генерации ID

public:
    BaseSensor(const IDataProvider* dataProvider, const std::string& type);

    std::string getType() const override;
    int getId() const override;

    virtual ~BaseSensor() = default;
};

