#pragma once

enum class ProgramState {
    PLACING_COMPONENT,
    CONNECTING_WIRE,
    IDLE,
    PANNING,
    SELECTING,
    MOVING_COMPONENT
};

enum class ComponentType {
    AND,
    OR,
    NOT,
    INPUT_SWITCH
};
