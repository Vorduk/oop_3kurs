#include <iostream>
#include <memory>
#include <vector>

#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"
#include "OldHeaterAdapter.h"
#include "OldHeater.h"
#include "DeviceInfoVisitor.h"

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "greenhouse control system with visitor" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // контейнер для всех устройств
    std::vector<std::shared_ptr<IDevice> > all_devices;

    // создание устройств
    std::shared_ptr<Heater> heater = std::make_shared<Heater>();
    all_devices.push_back(heater);

    std::shared_ptr<Conditioner> conditioner = std::make_shared<Conditioner>();
    all_devices.push_back(conditioner);

    std::shared_ptr<AirHumidifier> humidifier = std::make_shared<AirHumidifier>();
    all_devices.push_back(humidifier);

    std::shared_ptr<Irrigation> irrigation = std::make_shared<Irrigation>();
    all_devices.push_back(irrigation);

    std::shared_ptr<Ventilation> ventilation = std::make_shared<Ventilation>();
    all_devices.push_back(ventilation);

    std::shared_ptr<Lamp> lamp = std::make_shared<Lamp>();
    all_devices.push_back(lamp);


    // настройка некоторых устройств для демонстрации
    heater->turnOn();
    heater->setPower(75);

    conditioner->turnOn();
    conditioner->setPower(40);

    humidifier->turnOn();
    humidifier->setMode(AirHumidifier::MODE_HIGH);

    irrigation->turnOn();
    irrigation->setMode(Irrigation::MODE_SPRINKLER);

    ventilation->turnOn();
    ventilation->setMode(Ventilation::MODE_HIGH);

    lamp->turnOn();
    lamp->setPower(90);
    lamp->setMode(Lamp::MODE_WARM);

    // создание посетителя и обход всех устройств
    DeviceInfoVisitor visitor;
    std::cout << "\nvisitor: visit all devices" << std::endl;
    for (size_t i = 0; i < all_devices.size(); ++i) {
        all_devices[i]->accept(&visitor);
        std::cout << std::endl;
    }

    return 0;
}