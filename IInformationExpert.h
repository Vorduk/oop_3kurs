#pragma once

/**
 * @brief Интерфейс информационного эксперта
 */
class IInformationExpert {
public:
    virtual ~IInformationExpert() = default;
    virtual double getValue() = 0;
};