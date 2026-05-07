#include "RemoteControl.h"

void RemoteControl::setCommand(int slot, std::shared_ptr<ICommand> command) {
    m_commands[slot] = command;
}

void RemoteControl::pressButton(int slot) {
    auto it = m_commands.find(slot);
    if (it != m_commands.end() && it->second) {
        it->second->execute();
    }
}