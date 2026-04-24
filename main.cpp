#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "SimulationModel.h"
#include "LoggingObserver.h"
#include "AlertObserver.h"

int main() {
    std::cout << "Observer Pattern Demonstration" << std::endl;

    // Создаётся модель с начальными параметрами: температура 22.0 C, влажность 65%, почва 45%
    std::shared_ptr<SimulationModel> model =
        std::make_shared<SimulationModel>(22.0, 65.0, 45.0);

    // Создаются наблюдатели
    std::shared_ptr<LoggingObserver> logger = std::make_shared<LoggingObserver>();
    std::shared_ptr<AlertObserver> alerter = std::make_shared<AlertObserver>(10.0, 30.0, 80.0);

    // Наблюдатели подписываются на изменения модели
    model->attach(logger);
    model->attach(alerter);

    std::cout << "Observers attached. Starting parameter changes..." << std::endl;

    // Имитация работы системы: несколько шагов с изменением параметров
    // Шаг 1: нагрев
    std::cout << "\nStep 1: heater at 50%" << std::endl;
    model->applyHeaterEffect(50);
    model->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Шаг 2: увлажнение воздуха
    std::cout << "\nStep 2: humidifier at 80%" << std::endl;
    model->applyHumidifierEffect(80);
    model->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Шаг 3: полив почвы
    std::cout << "\nStep 3: irrigation at 60%" << std::endl;
    model->applyIrrigationEffect(60);
    model->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Шаг 4: кондиционер и вентиляция (охлаждение и осушение)
    std::cout << "\nStep 4: conditioner at 70%, ventilation at 40%" << std::endl;
    model->applyConditionerEffect(70);
    model->applyVentilationEffect(40);
    model->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Отписка одного наблюдателя для демонстрации
    std::cout << "\nDetaching alerter..." << std::endl;
    model->detach(alerter);

    // Ещё одно изменение, теперь alerter не должен реагировать
    std::cout << "\nStep 5: heater at 90% (after alerter detached)" << std::endl;
    model->applyHeaterEffect(90);
    model->update();

    return 0;
}