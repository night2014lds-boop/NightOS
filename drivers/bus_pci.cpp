#include <iostream>
#include <fstream>
#include <string>

class NightPciBus {
public:
    void scan() {
        std::ifstream file("/proc/bus/pci/devices");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {}
            file.close();
        }
    }
};

int main() {
    NightPciBus pci;
    pci.scan();
    return 0;
}
