#pragma once
#include <memory>

/**
 * @brief Интерфейс поставщика данных о состоянии теплицы
 *
 * Предоставляет доступ к текущим параметрам микроклимата:
 * температура воздуха, влажность воздуха и влажность почвы.
 * Этот интерфейс используется датчиками для получения актуальных показаний.
 * Реализуется классом SimulationModel, который симулирует физические процессы.
 * 
 * @note Все методы константные, так как чтение данных не должно изменять состояние
 */
class IDataProvider {
public:
    virtual ~IDataProvider() = default;

    /**
     * @brief Получить текущую температуру воздуха
     * @return Температура в градусах Цельсия
     */
    virtual double getTemperature() const = 0;

    /**
     * @brief Получить текущую влажность воздуха
     * @return Относительная влажность в процентах (0-100)
     */
    virtual double getAirHumidity() const = 0;

    /**
     * @brief Получить текущую влажность почвы
     * @return Влажность почвы в процентах (0-100)
     */
    virtual double getSoilMoisture() const = 0;
};

/**
 * @brief Тип-псевдоним для умного указателя на IDataProvider
 *
 * Использует std::shared_ptr для автоматического управления временем жизни.
 * Датчики хранят std::weak_ptr<IDataProvider>, чтобы избежать циклических ссылок.
 */
using IDataProviderPtr = std::shared_ptr<IDataProvider>;