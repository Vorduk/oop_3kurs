#pragma once
#include <map>
#include <string>

class ClimateManager;

class ISystemState {
public:
    virtual ~ISystemState() = default;

    // Обработка цикла управления.
    // readings - текущие показания датчиков,
    // targets - целевые параметры,
    // context - ссылка на контекст, чтобы состояние могло переключиться.
    virtual std::map<std::string, int> handle(
        const std::map<std::string, double>& readings,
        const std::map<std::string, double>& targets,
        ClimateManager* context) = 0;

    // Имя состояния для отладки.
    virtual std::string getName() const = 0;
};