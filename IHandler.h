#pragma once
#include <string>
#include <memory>

class IHandler {
public:
    virtual ~IHandler() = default;
    virtual void set_next(std::shared_ptr<IHandler> handler) = 0;
    virtual std::string handle(const std::string& signal, double value) = 0;
};