#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <string>

class BaseSensor : public ISensor {

protected:
    const IDataProvider* m_dataProvider;
    std::string m_type;

public:
    BaseSensor(const IDataProvider* dataProvider, const std::string& type)
        : m_dataProvider(dataProvider), m_type(type) {
    }

    std::string getType() const override {
        return m_type;
    }

    virtual ~BaseSensor() = default;
};

