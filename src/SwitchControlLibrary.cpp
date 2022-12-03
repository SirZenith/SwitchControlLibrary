#include "SwitchControlLibrary.h"

#if defined(_USING_HID)

static const uint8_t _hidReportDescriptor[] PROGMEM = {
    0x05, 0x01,       //   USAGE_PAGE (Generic Desktop)
    0x09, 0x05,       //   USAGE (Game Pad)
    0xa1, 0x01,       //   COLLECTION (Application)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x25, 0x01,       //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,       //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,       //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,       //   REPORT_SIZE (1)
    0x95, 0x10,       //   REPORT_COUNT (16)
    0x05, 0x09,       //   USAGE_PAGE (button)
    0x19, 0x01,       //   USAGE_MINIMUM (1)
    0x29, 0x10,       //   USAGE_MAXIMUM (16)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x05, 0x01,       //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,       //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01, //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,       //   REPORT_SIZE (4)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x65, 0x14,       //   UNIT (20)
    0x09, 0x39,       //   USAGE (hat Switch)
    0x81, 0x42,       //   INPUT (Data,Var,Abs)
    0x65, 0x00,       //   UNIT (0)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x81, 0x01,       //   INPUT (Cnst,Arr,Abs)
    0x26, 0xff, 0x00, //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00, //   PHYSICAL_MAXIMUM (255)
    0x09, 0x30,       //   USAGE (X)
    0x09, 0x31,       //   USAGE (Y)
    0x09, 0x32,       //   USAGE (Z)
    0x09, 0x35,       //   USAGE (Rz)
    0x75, 0x08,       //   REPORT_SIZE (8)
    0x95, 0x04,       //   REPORT_COUNT (4)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x06, 0x00, 0xff, //   USAGE_PAGE (Vendor Defined 65280)
    0x09, 0x20,       //   USAGE (32)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x0a, 0x21, 0x26, //   USAGE (9761)
    0x95, 0x08,       //   REPORT_COUNT (8)
    0x91, 0x02,       //   OUTPUT (Data,Var,Abs)
    0xc0              // END_COLLECTION
};

switch_controller::KeyType switch_controller::GetTypeInKeyCode(KeyCode code) {
    return KeyType(((unsigned long)code >> 16) & 0xFFFF);
}

uint16_t switch_controller::GetValueInKeyCode(KeyCode code) {
    return (unsigned long)code & 0xFFFF;
}

const char *switch_controller::GetNameOfKeyCode(KeyCode code) {
    KeyType type = GetTypeInKeyCode(code);

    const char *name = nullptr;
    switch (type) {
    case KeyType::BTN:
        name = "btn";
        break;
    case KeyType::HAT:
        name = "hat";
        break;
    case KeyType::HAT_BTN:
        name = "hat-btn";
        break;
    case KeyType::L_STICK:
        name = "l-stick";
        break;
    case KeyType::R_STICK:
        name = "r-stick";
        break;
        
    default:
        name = "unknown";
        break;
    }

    return name;
}

// ----------------------------------------------------------------------------

switch_controller::HatStack::HatStack() {
    buttons = new uint8_t[MAX_BTN_CAPABILITY];
}

switch_controller::HatStack::~HatStack() {
    delete[] buttons;
}

uint8_t switch_controller::HatStack::Get(int index) {
    uint8_t value = 0;
    if (index < 0 && -index <= top + 1) {
        value = buttons[top + 1 + index];
    } else if (index >= 0 && index < top + 1) {
        value = buttons[index];
    }

    return value;
}

int switch_controller::HatStack::Size() {
    return top + 1;
}

bool switch_controller::HatStack::Push(uint8_t btn) {
    bool ok = false;

    if (Contains(btn) < 0 and top + 1 < MAX_BTN_CAPABILITY) {
        ok = true;
        ++top;
        buttons[top] = btn;
    }

    return ok;
}

void switch_controller::HatStack::Erase(uint8_t btn) {
    int index = Contains(btn);

    if (index >= 0) {
        for (int i = index; i < top; ++i) {
            buttons[i] = buttons[i + 1];
        }
        --top;
    }
}

int switch_controller::HatStack::Contains(uint8_t btn) {
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

uint8_t switch_controller::HatState::Press(uint8_t hatButton) {
    hatStack.Push(hatButton);
    return (int8_t)GetHat();
}

uint8_t switch_controller::HatState::Release(uint8_t hatButton) {
    hatStack.Erase(hatButton);
    return (int8_t)GetHat();
}

// ----------------------------------------------------------------------------

switch_controller::SwitchController::SwitchController() {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    CustomHID().AppendDescriptor(&node);

    memset(&inputData, 0, sizeof(USB_JoystickReport_Input_t));

    inputData.lx = (int8_t)Stick::NEUTRAL;
    inputData.ly = (int8_t)Stick::NEUTRAL;
    inputData.rx = (int8_t)Stick::NEUTRAL;
    inputData.ry = (int8_t)Stick::NEUTRAL;
    inputData.hat = (int8_t)Hat::NEUTRAL;
}

void switch_controller::SwitchController::SendReport() {
    CustomHID().SendReport(&inputData, sizeof(USB_JoystickReport_Input_t));
}

void switch_controller::SwitchController::Press(KeyCode code) {
    KeyType type = GetTypeInKeyCode(code);
    uint16_t value = GetValueInKeyCode(code);

    switch (type) {
    case KeyType::BTN:
        PressButton(value);
        break;
    case KeyType::HAT:
        MoveHat(value);
        break;
    case KeyType::HAT_BTN:
        PressHatButton(value);
        break;
    case KeyType::L_STICK:
        MoveLeftStick(
            (value >> sizeof(uint8_t)) & 0xFF,
            value & 0xFF
        );
        break;
    case KeyType::R_STICK:
        MoveRightStick(
            (value >> sizeof(uint8_t)) & 0xFF,
            value & 0xFF
        );
        break;

    default:
        break;
    }
}

void switch_controller::SwitchController::Release(KeyCode code) {
    KeyType type = GetTypeInKeyCode(code);
    uint16_t value = GetValueInKeyCode(code);

    switch (type) {
    case KeyType::BTN:
        ReleaseButton(value);
        break;
    case KeyType::HAT:
        MoveHat((int8_t)Hat::NEUTRAL);
        break;
    case KeyType::HAT_BTN:
        ReleaseHatButton(value);
        break;
    case KeyType::L_STICK:
        MoveLeftStick(
            (int8_t)Stick::NEUTRAL,
            (int8_t)Stick::NEUTRAL
        );
        break;
    case KeyType::R_STICK:
        MoveRightStick(
            (int8_t)Stick::NEUTRAL,
            (int8_t)Stick::NEUTRAL
        );

        break;

    default:
        break;
    }
}

void switch_controller::SwitchController::Press(unsigned long code) {
    Press((KeyCode)code);
}

void switch_controller::SwitchController::Release(unsigned long code) {
    Release((KeyCode)code);
}
// ----------------------------------------------------------------------------

void switch_controller::SwitchController::PressButton(uint16_t button) {
    inputData.button |= button;
}

void switch_controller::SwitchController::ReleaseButton(uint16_t button) {
    inputData.button &= (button ^ 0xFFFF);
}

// ----------------------------------------------------------------------------

void switch_controller::SwitchController::MoveHat(uint8_t hat) {
    inputData.hat = hat;
}

void switch_controller::SwitchController::PressHatButton(uint8_t hatButton) {
    inputData.hat = hatState.Press(hatButton);
}

void switch_controller::SwitchController::ReleaseHatButton(uint8_t hatButton) {
    inputData.hat = hatState.Release(hatButton);
}

// ----------------------------------------------------------------------------

void switch_controller::SwitchController::MoveLeftStick(uint8_t x, uint8_t y) {
    inputData.lx = x;
    inputData.ly = y;
}

void switch_controller::SwitchController::MoveRightStick(uint8_t x, uint8_t y) {
    inputData.rx = x;
    inputData.ry = y;
}

// ----------------------------------------------------------------------------

switch_controller::SwitchController &SwitchController() {
    static switch_controller::SwitchController obj;
    return obj;
}

#endif
