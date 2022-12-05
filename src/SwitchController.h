#pragma once

#include <Arduino.h>

#include <Adafruit_TinyUSB.h>
#include <tusb.h>
#include <tusb_config.h>
#include <tusb_option.h>

#include "HatState.h"
#include "keycode.h"

namespace switch_controller {
    static const uint8_t HID_REPORT_DESCRIPTOR[] PROGMEM = {
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

    KeyType GetTypeInKeyCode(KeyCode code);
    uint16_t GetValueInKeyCode(KeyCode code);
    const char *GetNameOfKeyCode(KeyCode code);

    struct USB_JoystickReport_Data_t {
        uint16_t button;
        uint8_t hat;
        uint8_t lx;
        uint8_t ly;
        uint8_t rx;
        uint8_t ry;
        uint8_t vendorSpec;
    };

    // ------------------------------------------------------------------------

    class SwitchController {
    public:
        SwitchController();

        void SendReport();

        void Press(KeyCode code);
        void Release(KeyCode code);

        void Press(unsigned long code);
        void Release(unsigned long code);

        // --------------------------------------------------------------------

        void PressButton(Button button);
        void ReleaseButton(Button button);

        void SetCross(Cross cross);

        void PressCrossButton(CrossButton btn);
        void ReleaseCrossButton(CrossButton btn);

        void MoveLStick(uint8_t x, uint8_t y);
        void MoveRStick(uint8_t x, uint8_t y);

    private:
        USB_JoystickReport_Data_t inputData;
        HatState hatState;
        Adafruit_USBD_HID hid;
    };

    static SwitchController controller;
};
