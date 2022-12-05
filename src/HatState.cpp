#include "HatState.h"

// ----------------------------------------------------------------------------

switch_controller::HatState::HatStack::HatStack() {
    buttons = new uint8_t[MAX_BTN_CAPABILITY];
}

switch_controller::HatState::HatStack::~HatStack() {
    delete[] buttons;
}

uint8_t switch_controller::HatState::HatStack::Get(int index) {
    uint8_t value = 0;
    if (index < 0 && -index <= top + 1) {
        value = buttons[top + 1 + index];
    } else if (index >= 0 && index < top + 1) {
        value = buttons[index];
    }

    return value;
}

int switch_controller::HatState::HatStack::Size() {
    return top + 1;
}

bool switch_controller::HatState::HatStack::Push(uint8_t btn) {
    bool ok = false;

    if (Contains(btn) < 0 and top + 1 < MAX_BTN_CAPABILITY) {
        ok = true;
        ++top;
        buttons[top] = btn;
    }

    return ok;
}

void switch_controller::HatState::HatStack::Erase(uint8_t btn) {
    int index = Contains(btn);

    if (index >= 0) {
        for (int i = index; i < top; ++i) {
            buttons[i] = buttons[i + 1];
        }
        --top;
    }
}

int switch_controller::HatState::HatStack::Contains(uint8_t btn) {
    int isFound = -1;

    for (int i = 0; i <= top; ++i) {
        if (buttons[i] == btn) {
            isFound = i;
            break;
        }
    }

    return isFound;
}

// ----------------------------------------------------------------------------

switch_controller::Hat switch_controller::HatState::GetHat() {
    uint8_t hatButton = 0b0000;
    // taks only 2 latest pressed key
    for (int i = 1; i <= 2; ++i) {
        hatButton |= hatStack.Get(-i);
    }

    Hat hat = Hat::NEUTRAL;
    switch (hatButton) {
    case 0b0000:
        hat = Hat::NEUTRAL;
        break;
    case 0b0001:
        hat = Hat::UP;
        break;
    case 0b0010:
        hat = Hat::RIGHT;
        break;
    case 0b0011:
        hat = Hat::UP_RIGHT;
        break;
    case 0b0100:
        hat = Hat::DOWN;
        break;
    case 0b0110:
        hat = Hat::DOWN_RIGHT;
        break;
    case 0b1000:
        hat = Hat::LEFT;
        break;
    case 0b1001:
        hat = Hat::UP_LEFT;
        break;
    case 0b1100:
        hat = Hat::DOWN_LEFT;
        break;
    default:
        hat = Hat::NEUTRAL;
        break;
    }
    return hat;
}

switch_controller::HatState::HatState() {}

switch_controller::Hat switch_controller::HatState::Press(uint8_t hatButton) {
    hatStack.Push(hatButton);
    return GetHat();
}

switch_controller::Hat switch_controller::HatState::Release(uint8_t hatButton) {
    hatStack.Erase(hatButton);
    return GetHat();
}

