#include "BaseSensor.h"

int BaseSensor::s_next_id = 1; // Начальное значение id сенсора.

BaseSensor::BaseSensor(std::weak_ptr<const IDataProvider> data_provider, const std::string& type)
    : m_data_provider(data_provider), 
    m_type(type), 
    m_id(s_next_id++)
{

}

std::string BaseSensor::getType() const
{
    return m_type;
}

int BaseSensor::getId() const
{
    return m_id;
}
