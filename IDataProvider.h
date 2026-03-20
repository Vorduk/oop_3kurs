#pragma once
#include <memory>

class IDataProvider {
public:
    virtual ~IDataProvider() = default;
    virtual double getTemperature() const = 0;
    virtual double getAirHumidity() const = 0;
    virtual double getSoilMoisture() const = 0;
};

using IDataProviderPtr = std::shared_ptr<IDataProvider>;