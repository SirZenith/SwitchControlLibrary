#pragma once

#include <Arduino.h>

namespace switch_controller {
    enum class Button : uint16_t {
        Y = 1 << 0,
        B = 1 << 1,
        A = 1 << 2,
        X = 1 << 3,
        L = 1 << 4,
        R = 1 << 5,
        ZL = 1 << 6,
        ZR = 1 << 7,
        MINUS = 1 << 8,
        PLUS = 1 << 9,
        LCLICK = 1 << 10,
        RCLICK = 1 << 11,
        HOME = 1 << 12,
        CAPTURE = 1 << 13,
    };

    enum class Cross : uint8_t {
        UP = 0,
        UP_RIGHT = 1,
        RIGHT = 2,
        DOWN_RIGHT = 3,
        DOWN = 4,
        DOWN_LEFT = 5,
        LEFT = 6,
        UP_LEFT = 7,
        NEUTRAL = 8,
    };

    enum class CrossButton : uint8_t {
        UP = 1 << 0,
        RIGHT = 1 << 1,
        DOWN = 1 << 2,
        LEFT = 1 << 3,
    };

    enum class Stick : uint8_t {
        MIN = 0,
        NEUTRAL = 128,
        MAX = 255,
    };

    enum class KeyType : uint16_t {
        BTN,
        CROSS,
        CROSS_BTN,
        L_STICK,
        R_STICK,
    };

    constexpr unsigned long GenKeyCode(const KeyType type, const uint16_t value) {
        return (unsigned long)(type) << 16 | (unsigned long)(value);
    }

    constexpr unsigned long GenBtnKeyCode(const uint16_t value) {
        return GenKeyCode(KeyType::BTN, value);
    }

    constexpr unsigned long GenHatKeyCode(const uint16_t value) {
        return GenKeyCode(KeyType::CROSS, value);
    }

    constexpr unsigned long GenHatBtnKeyCode(const uint16_t value) {
        return GenKeyCode(KeyType::CROSS_BTN, value);
    }

    constexpr unsigned long GenLeftStickKeyCode(const uint8_t x, const uint8_t y) {
        return GenKeyCode(KeyType::L_STICK, (uint16_t)(x) << sizeof(uint8_t) | (uint16_t)(y));
    }

    constexpr unsigned long GenLeftStickKeyCode(const Stick x, const Stick y) {
        return GenKeyCode(KeyType::L_STICK, (uint16_t)(x) << sizeof(uint8_t) | (uint16_t)(y));
    }

    constexpr unsigned long GenRightStickKeyCode(const uint8_t x, const uint8_t y) {
        return GenKeyCode(KeyType::R_STICK, (uint16_t)(x) << sizeof(uint8_t) | (uint16_t)(y));
    }

    constexpr unsigned long GenRightStickKeyCode(const Stick x, const Stick y) {
        return GenKeyCode(KeyType::R_STICK, (uint16_t)(x) << sizeof(uint8_t) | (uint16_t)(y));
    }

    enum class KeyCode : unsigned long {
        BTN_Y = GenBtnKeyCode((uint16_t)Button::Y),
        BTN_B = GenBtnKeyCode((uint16_t)Button::B),
        BTN_A = GenBtnKeyCode((uint16_t)Button::A),
        BTN_X = GenBtnKeyCode((uint16_t)Button::X),
        BTN_L = GenBtnKeyCode((uint16_t)Button::L),
        BTN_R = GenBtnKeyCode((uint16_t)Button::R),
        BTN_ZL = GenBtnKeyCode((uint16_t)Button::ZL),
        BTN_ZR = GenBtnKeyCode((uint16_t)Button::ZR),
        BTN_MINUS = GenBtnKeyCode((uint16_t)Button::MINUS),
        BTN_PLUS = GenBtnKeyCode((uint16_t)Button::PLUS),
        BTN_LCLICK = GenBtnKeyCode((uint16_t)Button::LCLICK),
        BTN_RCLICK = GenBtnKeyCode((uint16_t)Button::RCLICK),
        BTN_HOME = GenBtnKeyCode((uint16_t)Button::HOME),
        BTN_CAPTURE = GenBtnKeyCode((uint16_t)Button::CAPTURE),

        CROSS_UP = GenHatKeyCode((uint16_t)Cross::UP),
        CROSS_UP_RIGHT = GenHatKeyCode((uint16_t)Cross::UP_RIGHT),
        CROSS_RIGHT = GenHatKeyCode((uint16_t)Cross::RIGHT),
        CROSS_DOWN_RIGHT = GenHatKeyCode((uint16_t)Cross::DOWN_RIGHT),
        CROSS_DOWN = GenHatKeyCode((uint16_t)Cross::DOWN),
        CROSS_DOWN_LEFT = GenHatKeyCode((uint16_t)Cross::DOWN_LEFT),
        CROSS_LEFT = GenHatKeyCode((uint16_t)Cross::LEFT),
        CROSS_UP_LEFT = GenHatKeyCode((uint16_t)Cross::UP_LEFT),
        CROSS_NEUTRAL = GenHatKeyCode((uint16_t)Cross::NEUTRAL),

        CROSS_BTN_UP = GenHatBtnKeyCode((uint16_t)CrossButton::UP),
        CROSS_BTN_RIGHT = GenHatBtnKeyCode((uint16_t)CrossButton::RIGHT),
        CROSS_BTN_DOWN = GenHatBtnKeyCode((uint16_t)CrossButton::DOWN),
        CROSS_BTN_LEFT = GenHatBtnKeyCode((uint16_t)CrossButton::LEFT),
    };
};