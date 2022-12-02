#pragma once

#include "CustomHID.h"

struct Button {
    static const uint16_t Y = 0x0001;
    static const uint16_t B = 0x0002;
    static const uint16_t A = 0x0004;
    static const uint16_t X = 0x0008;
    static const uint16_t L = 0x0010;
    static const uint16_t R = 0x0020;
    static const uint16_t ZL = 0x0040;
    static const uint16_t ZR = 0x0080;
    static const uint16_t MINUS = 0x0100;
    static const uint16_t PLUS = 0x0200;
    static const uint16_t LCLICK = 0x0400;
    static const uint16_t RCLICK = 0x0800;
    static const uint16_t HOME = 0x1000;
    static const uint16_t CAPTURE = 0x2000;
};

struct Hat {
    static const uint8_t UP = 0x00;
    static const uint8_t UP_RIGHT = 0x01;
    static const uint8_t RIGHT = 0x02;
    static const uint8_t DOWN_RIGHT = 0x03;
    static const uint8_t DOWN = 0x04;
    static const uint8_t DOWN_LEFT = 0x05;
    static const uint8_t LEFT = 0x06;
    static const uint8_t UP_LEFT = 0x07;
    static const uint8_t NEUTRAL = 0x08;
};

struct HatButton {
    static const uint8_t UP = 0b0001;
    static const uint8_t RIGHT = 0b0010;
    static const uint8_t DOWN = 0b0100;
    static const uint8_t LEFT = 0b1000;
};

struct Stick {
    static const uint8_t MIN = 0;
    static const uint8_t NEUTRAL = 128;
    static const uint8_t MAX = 255;
};

struct USB_JoystickReport_Input_t {
    uint16_t Button;
    uint8_t Hat;
    uint8_t LX;
    uint8_t LY;
    uint8_t RX;
    uint8_t RY;
    uint8_t VendorSpec;
};

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

class HatState {
private:
    HatStack hatStack;

    uint8_t getHat();

public:
    HatState();

    uint8_t pressHatButton(uint8_t hat_button);
    uint8_t releaseHatButton(uint8_t hat_button);
};

class SwitchControlLibrary_ {
private:
    USB_JoystickReport_Input_t _joystickInputData;
    HatState _hatState;

public:
    SwitchControlLibrary_();

    void sendReport();

    void pressButton(uint16_t button);
    void releaseButton(uint16_t button);

    void moveHat(uint8_t hat);
    void pressHatButton(uint8_t hat_button);
    void releaseHatButton(uint8_t hat_button);

    void moveLeftStick(uint8_t lx, uint8_t ly);
    void moveRightStick(uint8_t rx, uint8_t ry);
};

SwitchControlLibrary_ &SwitchControlLibrary();
