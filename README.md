# Switch Controller Package

## Introduction

This package enable Arduino to act as a Switch controller.

## Prerequest

This package have following dependency(ies),
make sure you have the right version installed:

```
TinyUSB == 0.10.5
```

And before compiling and uploading your sketch onto you board.
You should change VID and PID in your boards.txt into one of following:

|            Device Name            |  VID   |  PID   |
|:---------------------------------:|:------:|:------:|
| HORI HORIPAD for Nintendo Switch  | 0x0f0d | 0x00c1 |
| HORI Pokken Tournament DX Pro Pad | 0x0f0d | 0x0092 |
|  Nintendo Switch Pro Controller   | 0x057e | 0x2009 |
|  Nintendo Switch Joy-Con (Right)  | 0x057e | 0x2007 |
|  Nintendo Switch Joy-Con (Left)   | 0x057e | 0x2006 |

## Key Type and Key Code

(Both `KeyType` and `KeyCode` are defined under name space `switch_controller`)

This library provide following key type:

```cpp
enum class KeyType {
    BTN,       // buttons other cross button.
    CROSS,     // possiblely combined severial cross button.
    CROSS_BTN, // one direction of cross button.
    L_STICK,   // left joy stick.
    R_STICK    // right joy stick.
};
```

A KeyCode's under lying type is an unsigned 32-bit int i.e. `unsigned long`.

Key code for joy stick need to be composed directly into a 32-bit int value,
for more detail read API section.
Key codes for buttons are given as follow:

```cpp
enum KeyCode : unsigned long {
    BTN_Y,
    BTN_B,
    BTN_A,
    BTN_X,
    BTN_L,
    BTN_R,
    BTN_ZL,
    BTN_ZR,
    BTN_MINUS,
    BTN_PLUS,
    BTN_LCLICK,
    BTN_RCLICK,
    BTN_HOME,
    BTN_CAPTURE,

    CROSS_UP,
    CROSS_UP_RIGHT,
    CROSS_RIGHT,
    CROSS_DOWN_RIGHT,
    CROSS_DOWN,
    CROSS_DOWN_LEFT,
    CROSS_LEFT,
    CROSS_UP_LEFT,
    CROSS_NEUTRAL,

    CROSS_BTN_UP,
    CROSS_BTN_RIGHT,
    CROSS_BTN_DOWN,
    CROSS_BTN_LEFT,
};
```
## API                     
                           
All APIs and type are defi ned under name space `switch_controller`.

APIs are encapsulated into static singleton `controller`.
It's of type `SwitchController`.

No key press/release will be send until you trigger report sending.
Remember to call `SendReport` on controller when you need you keys to take effect.

```cpp
using switch_controller::controller;

// press L and R button at the same time.
controller.Press(KeyCode::BTN_L);
controller.Press(KeyCode::BTN_R);
controller.SendReport();

delay(100);

controller.Release(KeyCode::BTN_L);
controller.Release(KeyCode::BTN_R);
controller.SendReport();
```

There are tow major parts of API.

### Specified

Each of APIs in this part deals with one specific key type.
And you don't need to use any KeyCode to call them.
At the mean time, this package provide enum type for each key type.

To convert between KeyCode and those dedicated type,
this package provides following functoins:

```cpp
// from KeyCode
KeyType GetTypeInKeyCode(KeyCode code);
uint16_t GetValueInKeyCode(KeyCode code);

// to KeyCode
constexpr unsigned long GenKeyCode(const KeyType type, const uint16_t value);
constexpr unsigned long GenBtnKeyCode(const uint16_t value);
constexpr unsigned long GenHatKeyCode(const uint16_t value);
constexpr unsigned long GenHatBtnKeyCode(const uint16_t value);
constexpr unsigned long GenLeftStickKeyCode(const uint8_t x, const uint8_t y);
constexpr unsigned long GenLeftStickKeyCode(const Stick x, const Stick y);
constexpr unsigned long GenRightStickKeyCode(const uint8_t x, const uint8_t y);
constexpr unsigned long GenRightStickKeyCode(const Stick x, const Stick y);
```

- Button

  ```cpp
  void PressButton(Button button);
  void ReleaseButton(Button button);

  enum class Button : uint16_t {
      Y,
      B,
      A,
      X,
      L,
      R,
      ZL,
      ZR,
      MINUS,
      PLUS,
      LCLICK,
      RCLICK,
      HOME,
      CAPTURE,
  };
  ```
- Cross Key

  When you need to press two direction at a time.

  ```cpp
  void SetCross(Cross cross);

  enum class Cross : uint8_t {
      UP,
      UP_RIGHT,
      RIGHT,
      DOWN_RIGHT,
      DOWN,
      DOWN_LEFT,
      LEFT,
      UP_LEFT,
      NEUTRAL,
  };
  ```

  When only 1 direction is needed:

  ```cpp
  void PressCrossButton(CrossButton btn);
  void ReleaseCrossButton(CrossButton btn);

  enum class CrossButton : uint8_t {
      UP,
      RIGHT,
      DOWN,
      LEFT,
  };
  ```
- Joy Stick

  ```cpp
  void MoveLStick(uint8_t x, uint8_t y);
  void MoveRStick(uint8_t x, uint8_t y);

  enum class Stick : uint8_t {
      MIN = 0,
      NEUTRAL = 128,
      MAX = 255,
  };
  ```

  x and y are value between 0 and 255.
  For x, 0 means left most, 255 means right most;
  for y, 0 is bottom, 255 is top.

### Aggregate

In this part of APIs, you pass in a key code,
then the package will switch to the corressponding handler.

Joy stick code need to be generated using convert function methioned in last subsection.

```cpp
void SwitchController::Press(KeyCode code);
void SwitchController::Release(KeyCode code);

void SwitchController::Press(unsigned long code);
void SwitchController::Release(unsigned long code);
```

## License

[MIT](https://github.com/SirZenith/SwitchControlLibrary/blob/master/LICENSE)
