#include "BaseDevice.h"

int BaseDevice::s_next_id = 1;

BaseDevice::BaseDevice(const std::string& type)
	: m_type(type), m_id(s_next_id++)
{

}

std::string BaseDevice::getType() const
{
	return m_type;
}

int BaseDevice::getId() const
{
	return m_id;
}
