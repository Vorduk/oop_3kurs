#include "BaseSensor.h"

int BaseSensor::s_next_id = 1;

BaseSensor::BaseSensor(std::weak_ptr<const IDataProvider> data_provider, const std::string& type)
    : m_data_provider(data_provider), m_type(type), m_id(s_next_id++)
{
}

// Конструктор копирования
BaseSensor::BaseSensor(const BaseSensor& other)
    : m_data_provider(other.m_data_provider)
    , m_type(other.m_type)
    , m_id(s_next_id++)        // новый уникальный ID
{
    std::cout << "[BaseSensor] Copy constructor: created clone of type '"
        << m_type << "' with new ID=" << m_id << std::endl;
}

std::string BaseSensor::getType() const {
    return m_type;
}

int BaseSensor::getId() const {
    return m_id;
}