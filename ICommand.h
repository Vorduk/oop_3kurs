#pragma once

// Интерфейс команды
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};