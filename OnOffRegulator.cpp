#include "OnOffRegulator.h"

double OnOffRegulator::calculate(double current, double target)
{
    if (current < target - m_hysteresis) {
        m_lastState = true;
        return 100.0;  // Включить на полную
    }
    else if (current > target + m_hysteresis) {
        m_lastState = false;
        return 0.0;    // Выключить
    }

    // В гистерезисе - предыдущее состояние
    return m_lastState ? 100.0 : 0.0;
}

std::string OnOffRegulator::getName() const
{
    return "OnOffRegulator";
}

void OnOffRegulator::setHysteresis(double hysteresis)
{
    m_hysteresis = hysteresis;
}
