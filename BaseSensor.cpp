#include "BaseSensor.h"

int BaseSensor::s_next_id = 1;

BaseSensor::BaseSensor(const IDataProvider* dataProvider, const std::string& type)
    : m_dataProvider(dataProvider), 
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
