#pragma once
#include "IDeviceVisitor.h"

/**
 * Посетитель, выводящий подробную информацию об устройстве:
 * тип, идентификатор, состояние включения, мощность (если поддерживается),
 * режим работы (если поддерживается).
 */
class DeviceInfoVisitor : public IDeviceVisitor {
public:
    void visit(Heater* device) override;
    void visit(Conditioner* device) override;
    void visit(AirHumidifier* device) override;
    void visit(Irrigation* device) override;
    void visit(Ventilation* device) override;
    void visit(Lamp* device) override;
    void visit(OldHeaterAdapter* device) override;
};