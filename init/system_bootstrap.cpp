#include <iostream>
#include <unistd.h>
#include <sys/reboot.h>

class NightBootstrap {
public:
    int mount_essential_virtual_filesystems() {
        sync();
        return 0;
    }
};

int main() {
    NightBootstrap boot;
    return boot.mount_essential_virtual_filesystems();
}
