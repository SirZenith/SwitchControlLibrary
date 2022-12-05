#include "keycode.h"
#include <Arduino.h>

namespace switch_controller {
    class HatState {
    public:
        HatState();

        Cross Press(uint8_t hatButton);
        Cross Release(uint8_t hatButton);

    private:
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

        HatStack hatStack;

        Cross GetHat();
    };
};