#include "OldHeater.h"

OldHeater::OldHeater() : m_heat_level(0), m_power(false)
{
}

void OldHeater::powerOn()
{
	m_power = true;
}

void OldHeater::powerOff()
{
	m_power = false;
}

void OldHeater::setHeatLevel(int level)
{
    if (level < 0 || level > 10) {
        return;
    }
    if (m_power) {
        m_heat_level = level;
    }
}

int OldHeater::getHeatLevel()
{
	return m_heat_level;
}
