#pragma once
#include "IDevice.h"
#include <string>

class BaseDevice : public IDevice {
protected:
    std::string m_type;
    int m_id;
    static int s_next_id;  // Статический счетчик для генерации ID

public:
    BaseDevice(const std::string& type);
    virtual ~BaseDevice() = default;

    std::string getType() const override;
    int getId() const override;
};

