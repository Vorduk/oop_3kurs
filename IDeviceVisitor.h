#pragma once

class Heater;
class Conditioner;
class AirHumidifier;
class Irrigation;
class Ventilation;
class Lamp;
class OldHeaterAdapter;

/**
 * Интерфейс посетителя для устройств.
 * Каждый конкретный посетитель реализует набор методов visit() для разных типов устройств.
 */
class IDeviceVisitor {
public:
    virtual ~IDeviceVisitor() = default;

    virtual void visit(Heater* device) = 0;
    virtual void visit(Conditioner* device) = 0;
    virtual void visit(AirHumidifier* device) = 0;
    virtual void visit(Irrigation* device) = 0;
    virtual void visit(Ventilation* device) = 0;
    virtual void visit(Lamp* device) = 0;
    virtual void visit(OldHeaterAdapter* device) = 0;
};