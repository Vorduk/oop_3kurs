// BaseSensor.h
#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <string>
#include <memory>

class BaseSensor : public ISensor {
protected:
    std::weak_ptr<const IDataProvider> m_data_provider;
    std::string m_type;
    int m_id;
    static int s_next_id;

public:
    BaseSensor(std::weak_ptr<const IDataProvider> data_provider, const std::string& type);

    // Конструктор
    BaseSensor(const BaseSensor& other);

    virtual ~BaseSensor() = default;

    std::string getType() const override;
    int getId() const override;

    // clone() остаётся чисто виртуальным, реализуется в потомках
    virtual std::shared_ptr<ISensor> clone() const override = 0;
};