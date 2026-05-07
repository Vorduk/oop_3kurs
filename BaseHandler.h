#pragma once
#include "IHandler.h"
#include <memory>
#include <iostream>

class BaseHandler : public IHandler {
protected:
    std::shared_ptr<IHandler> m_next;

public:
    void set_next(std::shared_ptr<IHandler> handler) override {
        std::cout << "[BaseHandler]: Setting next handler" << std::endl;
        m_next = handler;
    }

protected:
    std::string pass_to_next(const std::string& signal, double value) {
        if (m_next) {
            std::cout << "[BaseHandler]: Passing to next handler" << std::endl;
            return m_next->handle(signal, value);
        }
        std::cout << "[BaseHandler]: No next handler, stopping chain" << std::endl;
        return "UNHANDLED";
    }
};