#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>

class NightInputSystem {
public:
    int configure_mouse_pointer() {
        if (std::system("command -v xset > /dev/null 2>&1") != 0) {
            return 1;
        }
        std::system("xset m 1/1 0");
        std::system("xset r rate 250 30");
        return 0;
    }

    int verify_input_drivers() {
        if (access("/dev/input/mice", F_OK) == 0) {
            return 0;
        }
        return 1;
    }
};

int main() {
    NightInputSystem input;
    if (input.verify_input_drivers() == 0) {
        input.configure_mouse_pointer();
    }
    return 0;
}
