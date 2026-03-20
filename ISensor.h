#pragma once
#include <string>

// Интерфейс сенсора
class ISensor
{
public:
	virtual ~ISensor() = default; // Виртуальный деструктор
	virtual double getValue() = 0; // Виртуальный метод для получения показания с датчика.
	virtual std::string getType() const = 0;
};

