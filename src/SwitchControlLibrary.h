#pragma once

#include "CustomHID.h"
#include "keycode.h"
#include <Arduino.h>

namespace switch_controller {
    KeyType GetTypeInKeyCode(KeyCode code);
    uint16_t GetValueInKeyCode(KeyCode code);
    const char *GetNameOfKeyCode(KeyCode code);

    struct USB_JoystickReport_Input_t {
        uint16_t button;
        uint8_t hat;
        uint8_t lx;
        uint8_t ly;
        uint8_t rx;
        uint8_t ry;
        uint8_t vendorSpec;
    };

    // ------------------------------------------------------------------------

    class HatStack {
    private:
        uint8_t *buttons;
        int top = -1;

    public:
        // hat button can give 8 direction in total
        static const int MAX_BTN_CAPABILITY = 8;

        HatStack();
        ~HatStack();

        uint8_t Get(int index);
        int Size();
        int Contains(uint8_t btn);

        bool Push(uint8_t btn);
        void Erase(uint8_t btn);
    };

    // ------------------------------------------------------------------------

    class HatState {
    private:
        HatStack hatStack;

        Hat GetHat();

    public:
        HatState();

        uint8_t Press(uint8_t hatButton);
        uint8_t Release(uint8_t hatButton);
    };

    // ------------------------------------------------------------------------

    class SwitchController {
    private:
        USB_JoystickReport_Input_t inputData;
        HatState hatState;

    public:
        SwitchController();

        void SendReport();

        void Press(KeyCode code);
        void Release(KeyCode code);

        void Press(unsigned long code);
        void Release(unsigned long code);

        // --------------------------------------------------------------------

        void PressButton(uint16_t button);
        void ReleaseButton(uint16_t button);

        void MoveHat(uint8_t hat);
        void PressHatButton(uint8_t hatButton);
        void ReleaseHatButton(uint8_t hatButton);

        void MoveLeftStick(uint8_t x, uint8_t y);
        void MoveRightStick(uint8_t x, uint8_t y);
    };

};

switch_controller::SwitchController &SwitchController();