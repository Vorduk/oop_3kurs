#pragma once
#include "ISensorLogic.h"
#include "IDataProvider.h"
#include <memory>

/**
 * @brief Разделяемая логика датчика влажности почвы (ConcreteFlyweight)
 */
class SoilMoistureLogic : public ISensorLogic {
public:
    SoilMoistureLogic(std::weak_ptr<const IDataProvider> provider);
    double readValue() override;
    std::string getType() const override;

private:
    std::weak_ptr<const IDataProvider> m_data_provider;
};