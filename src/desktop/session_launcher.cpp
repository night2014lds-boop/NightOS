#include <iostream>
#include <cstdlib>
#include <unistd.h>

class NightSessionLauncher {
public:
    int launch_environment() {
        if (getuid() == 0) {
            return 1;
        }
        std::system("xinit /usr/bin/xfce4-session");
        return 0;
    }
};

int main() {
    NightSessionLauncher session;
    session.launch_environment();
    return 0;
}
