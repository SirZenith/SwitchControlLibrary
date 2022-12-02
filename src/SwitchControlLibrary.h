#pragma once

#include "CustomHID.h"

enum KeyType : uint16_t {
    BTN,
    HAT,
    HAT_BTN,
    L_STICK,
    R_STICK
};

constexpr unsigned long GenKeyCode(const KeyType type, const uint16_t value) {
    return type << 16 | value;
}

constexpr unsigned long GenBtnKeyCode(const uint16_t value) {
    return GenKeyCode(KeyType::BTN, value);
}

constexpr unsigned long GenHatKeyCode(const uint16_t value) {
    return GenKeyCode(KeyType::HAT, value);
}

constexpr unsigned long GenHatBtnKeyCode(const uint16_t value) {
    return GenKeyCode(KeyType::HAT_BTN, value);
}

constexpr unsigned long GenLeftStickKeyCode(const uint8_t x, const uint8_t y) {
    return GenKeyCode(KeyType::L_STICK, x << sizeof(uint8_t) | y);
}

constexpr unsigned long GenRightStickKeyCode(const uint8_t x, const uint8_t y) {
    return GenKeyCode(KeyType::R_STICK, x << sizeof(uint8_t) | y);
}

enum KeyCode : unsigned long {
    BTN_Y = GenBtnKeyCode(1 << 0),
    BTN_B = GenBtnKeyCode(1 << 1),
    BTN_A = GenBtnKeyCode(1 << 2),
    BTN_X = GenBtnKeyCode(1 << 3),
    BTN_L = GenBtnKeyCode(1 << 4),
    BTN_R = GenBtnKeyCode(1 << 5),
    BTN_ZL = GenBtnKeyCode(1 << 6),
    BTN_ZR = GenBtnKeyCode(1 << 7),
    BTN_MINUS = GenBtnKeyCode(1 << 8),
    BTN_PLUS = GenBtnKeyCode(1 << 9),
    BTN_LCLICK = GenBtnKeyCode(1 << 10),
    BTN_RCLICK = GenBtnKeyCode(1 << 11),
    BTN_HOME = GenBtnKeyCode(1 << 12),
    BTN_CAPTURE = GenBtnKeyCode(1 << 13),

    HAT_UP = GenHatKeyCode(0),
    HAT_UP_RIGHT = GenHatKeyCode(1),
    HAT_RIGHT = GenHatKeyCode(2),
    HAT_DOWN_RIGHT = GenHatKeyCode(3),
    HAT_DOWN = GenHatKeyCode(4),
    HAT_DOWN_LEFT = GenHatKeyCode(5),
    HAT_LEFT = GenHatKeyCode(6),
    HAT_UP_LEFT = GenHatKeyCode(7),
    HAT_NEUTRAL = GenHatKeyCode(8),

    HAT_BTN_UP = GenHatBtnKeyCode(1 << 0),
    HAT_BTN_RIGHT = GenHatBtnKeyCode(1 << 1),
    HAT_BTN_DOWN = GenHatBtnKeyCode(1 << 2),
    HAT_BTN_LEFT = GenHatBtnKeyCode(1 << 3),
};

KeyType GetTypeInKeyCode(KeyCode code) {
    return KeyType((code >> 16) & 0xFFFF);
}

uint16_t GetValueInKeyCode(KeyCode code) {
    return code & 0xFFFF;
}

// ----------------------------------------------------------------------------

struct USB_JoystickReport_Input_t {
    uint16_t Button;
    uint8_t Hat;
    uint8_t LX;
    uint8_t LY;
    uint8_t RX;
    uint8_t RY;
    uint8_t VendorSpec;
};

// ----------------------------------------------------------------------------

class HatStack {
private:
    uint8_t *buttons;
    int top = -1;

public:
    // hat button can give 8 direction in total
    static const int MAX_BTN_CAPABILITY = 8;

    HatStack();
    ~HatStack();

    uint8_t get(int index);
    int size();
    bool contains(uint8_t btn);

    bool push(uint8_t btn);
    void erase(uint8_t btn);
};

// ----------------------------------------------------------------------------

class HatState {
private:
    HatStack hatStack;

    uint8_t getHat();

public:
    HatState();

    uint8_t pressHatButton(uint8_t hatButton);
    uint8_t releaseHatButton(uint8_t hatButton);
};

// ----------------------------------------------------------------------------

class SwitchControlLibrary_ {
private:
    USB_JoystickReport_Input_t joystickInputData;
    HatState hatState;

public:
    SwitchControlLibrary_();

    void sendReport();

    void Press(KeyCode code);
    void pressButton(uint16_t button);
    void releaseButton(uint16_t button);

    void moveHat(uint8_t hat);
    void pressHatButton(uint8_t hatButton);
    void releaseHatButton(uint8_t hatButton);

    void moveLeftStick(uint8_t lx, uint8_t ly);
    void moveRightStick(uint8_t rx, uint8_t ry);
};

SwitchControlLibrary_ &SwitchControlLibrary();
