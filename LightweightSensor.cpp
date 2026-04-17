#include "LightweightSensor.h"
#include <iostream>

int LightweightSensor::s_next_id = 1;

LightweightSensor::LightweightSensor(std::shared_ptr<ISensorLogic> logic)
    : m_id(s_next_id++), m_logic(logic) {
}

double LightweightSensor::getValue() {
    double value = m_logic->readValue();
    std::cout << "[LightweightSensor]: " << getType() << " [ID:" << m_id
        << "] Reading: " << value;

    if (getType() == "temperature") {
        std::cout << " C";
    }
    else {
        std::cout << " %";
    }
    std::cout << std::endl;

    return value;
}

std::string LightweightSensor::getType() const {
    return m_logic->getType();
}

int LightweightSensor::getId() const {
    return m_id;
}