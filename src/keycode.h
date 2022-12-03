enum Button : uint16_t {
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
    CAPTURE = 1 << 13
};

enum Hat : uint8_t {
    UP = 0,
    UP_RIGHT = 1,
    RIGHT = 2,
    DOWN_RIGHT = 3,
    DOWN = 4,
    DOWN_LEFT = 5,
    LEFT = 6,
    UP_LEFT = 7,
    NEUTRAL = 8
};

enum HatButton : uint8_t {
    UP = 1 << 0,
    RIGHT = 1 << 1,
    DOWN = 1 << 2,
    LEFT = 1 << 3,
};

enum Stick : uint8_t {
    MIN = 0,
    NEUTRAL = 128,
    MAX = 255
};

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
    BTN_Y = GenBtnKeyCode(Button::Y),
    BTN_B = GenBtnKeyCode(Button::B),
    BTN_A = GenBtnKeyCode(Button::A),
    BTN_X = GenBtnKeyCode(Button::X),
    BTN_L = GenBtnKeyCode(Button::L),
    BTN_R = GenBtnKeyCode(Button::R),
    BTN_ZL = GenBtnKeyCode(Button::ZL),
    BTN_ZR = GenBtnKeyCode(Button::ZR),
    BTN_MINUS = GenBtnKeyCode(Button::MINUS),
    BTN_PLUS = GenBtnKeyCode(Button::PLUS),
    BTN_LCLICK = GenBtnKeyCode(Button::LCLICK),
    BTN_RCLICK = GenBtnKeyCode(Button::RCLICK),
    BTN_HOME = GenBtnKeyCode(Button::HOME),
    BTN_CAPTURE = GenBtnKeyCode(Button::CAPTURE),

    HAT_UP = GenHatKeyCode(Hat::UP),
    HAT_UP_RIGHT = GenHatKeyCode(Hat::UP_RIGHT),
    HAT_RIGHT = GenHatKeyCode(Hat::RIGHT),
    HAT_DOWN_RIGHT = GenHatKeyCode(Hat::DOWN_RIGHT),
    HAT_DOWN = GenHatKeyCode(Hat::DOWN),
    HAT_DOWN_LEFT = GenHatKeyCode(Hat::DOWN_LEFT),
    HAT_LEFT = GenHatKeyCode(Hat::LEFT),
    HAT_UP_LEFT = GenHatKeyCode(Hat::UP_LEFT),
    HAT_NEUTRAL = GenHatKeyCode(Hat::NEUTRAL),

    HAT_BTN_UP = GenHatBtnKeyCode(HatButton::UP),
    HAT_BTN_RIGHT = GenHatBtnKeyCode(HatButton::RIGHT),
    HAT_BTN_DOWN = GenHatBtnKeyCode(HatButton::DOWN),
    HAT_BTN_LEFT = GenHatBtnKeyCode(HatButton::LEFT),
};

KeyType GetTypeInKeyCode(KeyCode code) {
    return KeyType((code >> 16) & 0xFFFF);
}

uint16_t GetValueInKeyCode(KeyCode code) {
    return code & 0xFFFF;
}

