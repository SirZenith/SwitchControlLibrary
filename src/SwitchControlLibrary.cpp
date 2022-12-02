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
    0x05, 0x09,       //   USAGE_PAGE (Button)
    0x19, 0x01,       //   USAGE_MINIMUM (1)
    0x29, 0x10,       //   USAGE_MAXIMUM (16)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x05, 0x01,       //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,       //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01, //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,       //   REPORT_SIZE (4)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x65, 0x14,       //   UNIT (20)
    0x09, 0x39,       //   USAGE (Hat Switch)
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

// ----------------------------------------------------------------------------

HatStack::HatStack() {
    buttons = new uint8_t[MAX_BTN_CAPABILITY];
}

HatStack::~HatStack() {
    delete[] buttons;
}

uint8_t HatStack::get(int index) {
    uint8_t value = 0;
    if (index < 0 && -index < top + 1) {
        value = buttons[top + index];
    } else if (index > 0 && index < top + 1) {
        value = buttons[index];
    }

    return value;
}

int HatStack::size() {
    return top + 1;
}

bool HatStack::push(uint8_t btn) {
    bool ok = false;

    if (!contains(btn) and top + 1 < MAX_BTN_CAPABILITY) {
        ok = true;
        ++top;
        buttons[top] = btn;
    }

    return ok;
}

void HatStack::erase(uint8_t btn) {
    for (int cur = 0; cur <= top; ++cur) {
        if (buttons[cur] != btn) {
            continue;
        }

        for (int i = cur; i < top; ++i) {
            buttons[i] = buttons[i + 1];
        }
        --top;
        break;
    }
}

bool HatStack::contains(uint8_t btn) {
    bool isFound = false;

    for (int i = 0; i <= top; ++i) {
        if (buttons[i] == btn) {
            isFound = true;
            break;
        }
    }

    return isFound;
}

// ----------------------------------------------------------------------------

uint8_t HatState::getHat() {
    uint8_t hat_button = 0b0000;
    // taks only 2 latest pressed key
    for (int i = 1; i <= 2; ++i) {
        hat_button |= hatStack.get(-i);
    }

    uint8_t hat = Hat::NEUTRAL;
    switch (hat_button) {
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

HatState::HatState() {}

uint8_t HatState::pressHatButton(uint8_t hatButton) {
    hatStack.push(hatButton);
    return getHat();
}

uint8_t HatState::releaseHatButton(uint8_t hatButton) {
    hatStack.erase(hatButton);
    return getHat();
}

// ----------------------------------------------------------------------------

SwitchControlLibrary_::SwitchControlLibrary_() {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    CustomHID().AppendDescriptor(&node);

    memset(&joystickInputData, 0, sizeof(USB_JoystickReport_Input_t));

    uint16_t neturalValue = GetValueInKeyCode(KeyCode::STICK_NEUTRAL);
    joystickInputData.LX = neturalValue;
    joystickInputData.LY = neturalValue;
    joystickInputData.RX = neturalValue;
    joystickInputData.RY = neturalValue;
    joystickInputData.Hat = GetValueInKeyCode(KeyCode::HAT_NEUTRAL);
}

void SwitchControlLibrary_::sendReport() {
    CustomHID().SendReport(&joystickInputData, sizeof(USB_JoystickReport_Input_t));
}

void SwitchControlLibrary_::Press(KeyCode code) {
    KeyType type = GetTypeInKeyCode(code);
    uint16_t value = GetValueInKeyCode(code);

    switch (type) {
    case KeyType::BTN:
        joystickInputData.Button |= value;
        break;
    case KeyType::HAT:
        joystickInputData.Hat = value;
        break;
    case KeyType::HAT_BTN:
        joystickInputData.Hat = hatState.pressHatButton(value);
        break;
    case KeyType::L_STICK:
        joystickInputData.LX = (value >> sizeof(uint8_t)) & 0xFF;
        joystickInputData.LY = value & 0xFF;
        break;
    case KeyType::R_STICK:
        joystickInputData.RX = (value >> sizeof(uint8_t)) & 0xFF;
        joystickInputData.RY = value & 0xFF;
        break;

    default:
        break;
    }
}

void SwitchControlLibrary_::pressButton(uint16_t button) {
    joystickInputData.Button |= button;
}

void SwitchControlLibrary_::releaseButton(uint16_t button) {
    joystickInputData.Button &= (button ^ 0xFFFF);
}

void SwitchControlLibrary_::moveHat(uint8_t hat) {
    joystickInputData.Hat = hat;
}

void SwitchControlLibrary_::pressHatButton(uint8_t hat_button) {
    joystickInputData.Hat = hatState.pressHatButton(hat_button);
}

void SwitchControlLibrary_::releaseHatButton(uint8_t hat_button) {
    joystickInputData.Hat = hatState.releaseHatButton(hat_button);
}

void SwitchControlLibrary_::moveLeftStick(uint8_t lx, uint8_t ly) {
    joystickInputData.LX = lx;
    joystickInputData.LY = ly;
}

void SwitchControlLibrary_::moveRightStick(uint8_t rx, uint8_t ry) {
    joystickInputData.RX = rx;
    joystickInputData.RY = ry;
}

SwitchControlLibrary_ &SwitchControlLibrary() {
    static SwitchControlLibrary_ obj;
    return obj;
}

#endif
