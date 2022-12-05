#include "SwitchControlLibrary.h"

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

switch_controller::SwitchController::SwitchController() {
    hid.setPollInterval(1);
    hid.setReportDescriptor(HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR));

    memset(&inputData, 0, sizeof(USB_JoystickReport_Data_t));

    inputData.lx = (int8_t)Stick::NEUTRAL;
    inputData.ly = (int8_t)Stick::NEUTRAL;
    inputData.rx = (int8_t)Stick::NEUTRAL;
    inputData.ry = (int8_t)Stick::NEUTRAL;
    inputData.hat = (int8_t)Hat::NEUTRAL;
}

void switch_controller::SwitchController::SendReport() {
    hid.sendReport(0, &inputData, sizeof(USB_JoystickReport_Data_t));
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
        MoveLStick(
            (value >> sizeof(uint8_t)) & 0xFF,
            value & 0xFF
        );
        break;
    case KeyType::R_STICK:
        MoveRStick(
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
        MoveLStick(
            (int8_t)Stick::NEUTRAL,
            (int8_t)Stick::NEUTRAL
        );
        break;
    case KeyType::R_STICK:
        MoveRStick(
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
    inputData.hat = (uint8_t)hatState.Press(hatButton);
}

void switch_controller::SwitchController::ReleaseHatButton(uint8_t hatButton) {
    inputData.hat = (uint8_t)hatState.Release(hatButton);
}

// ----------------------------------------------------------------------------

void switch_controller::SwitchController::MoveLStick(uint8_t x, uint8_t y) {
    inputData.lx = x;
    inputData.ly = y;
}

void switch_controller::SwitchController::MoveRStick(uint8_t x, uint8_t y) {
    inputData.rx = x;
    inputData.ry = y;
}

// ----------------------------------------------------------------------------

switch_controller::SwitchController &SwitchController() {
    static switch_controller::SwitchController obj;
    return obj;
}
