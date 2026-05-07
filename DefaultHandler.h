#pragma once
#include "BaseHandler.h"
#include <iostream>

class DefaultHandler : public BaseHandler {
public:
    std::string handle(const std::string& signal, double value) override {
        std::cout << "[DefaultHandler]: Received unknown signal '" << signal << "'" << std::endl;
        return "UNKNOWN_SIGNAL: " + signal;
    }
};