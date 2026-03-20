#pragma once
#include <string>

/**
 * @brief Интерфейс для всех типов датчиков
 *
 * Каждый датчик имеет уникальный идентификатор, тип и метод получения показаний.
 */

class ISensor {
public:
	virtual ~ISensor() = default; // Виртуальный деструктор

	/**
	 * @brief Получить текущее показание датчика
	 * @return Значение измеряемого параметра
	 */
	virtual double getValue() = 0;

	/**
	 * @brief Получить тип датчика
	 * @return Строковый идентификатор типа (temperature, air_humidity, soil_moisture)
	 */
	virtual std::string getType() const = 0;

	/**
	 * @brief Получить уникальный идентификатор датчика
	 * @return Числовой ID датчика
	 * 
	 * Уникальность обеспечивается наследниками.
	 */
	virtual int getId() const = 0;
};

