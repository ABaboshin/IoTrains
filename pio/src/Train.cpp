//
// Created by andrey.baboshin on 1/31/2024.
//

#include <string>
#include "Train.h"

Train::Train() : drv(10, 11) {}

void Train::ProcessCommand(const railschema::Command &command) const {
    if (command.get_function() == railschema::Function::MOVE_FORWARD) {
        const auto speed = stoi(command.get_value().value());
        drv.Forward(speed);
    }

    if (command.get_function() == railschema::Function::MOVE_BACKWARD) {
        const auto speed = stoi(command.get_value().value());
        drv.Backward(speed);
    }

    if (command.get_function() == railschema::Function::STOP) {
        drv.Stop();
    }
}
