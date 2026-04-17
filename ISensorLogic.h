#pragma once
#include <string>

/**
 * @brief Интерфейс логики датчика (Flyweight)
 *
 * Объекты, реализующие этот интерфейс, являются приспособленцами.
 * Они хранят внутреннее состояние и разделяются между многими датчиками.
 */
class ISensorLogic {
public:
    virtual ~ISensorLogic() = default;
    virtual double readValue() = 0;
    virtual std::string getType() const = 0;
};