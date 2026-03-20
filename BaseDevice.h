#pragma once
#include "IDevice.h"
#include <string>

// Базовый класс устройства, от него наследуются все устройства.
class BaseDevice : public IDevice {
public:
    BaseDevice(const std::string& type);
    virtual ~BaseDevice() = default;

    std::string getType() const override;
    int getId() const override;

protected:
    std::string m_type;
    int m_id;
    static int s_next_id;  // Статический счетчик для генерации ID

};

