#pragma once

#include "CustomHID.h"
#include "keycode.h"

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

    Hat GetHat();

public:
    HatState();

    uint8_t Press(uint8_t hatButton);
    uint8_t Release(uint8_t hatButton);
};

// ----------------------------------------------------------------------------

class SwitchControlLibrary_ {
private:
    USB_JoystickReport_Input_t joystickInputData;
    HatState hatState;

public:
    SwitchControlLibrary_();

    void SendReport();

    void Press(KeyCode code);
    void Release(KeyCode code);

    // ------------------------------------------------------------------------

    void PressButton(uint16_t button);
    void ReleaseButton(uint16_t button);

    void MoveHat(uint8_t hat);
    void PressHatButton(uint8_t hatButton);
    void ReleaseHatButton(uint8_t hatButton);

    void MoveLeftStick(uint8_t x, uint8_t y);
    void MoveRightStick(uint8_t x, uint8_t y);
};

SwitchControlLibrary_ &SwitchControlLibrary();
