#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <cstring>

struct PciDevice {
    std::string address;
    std::string vendor_id;
    std::string device_id;
};

class NightPciScanner {
private:
    const std::string pci_base_path = "/sys/bus/pci/devices/";

    std::string read_hex_node(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "0x0000";
        std::string hex_val;
        file >> hex_val;
        file.close();
        return hex_val;
    }

public:
    std::vector<PciDevice> execute_bus_scan() {
        std::vector<PciDevice> device_list;
        DIR* dir = opendir(pci_base_path.c_str());
        if (!dir) return device_list;
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (std::strcmp(entry->d_name, ".") == 0 || std::strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            std::string dev_dir = pci_base_path + entry->d_name + "/";
            PciDevice dev;
            dev.address = entry->d_name;
            dev.vendor_id = read_hex_node(dev_dir + "vendor");
            dev.device_id = read_hex_node(dev_dir + "device");
            device_list.push_back(dev);
        }
        closedir(dir);
        return device_list;
    }
};

int main() {
    NightPciScanner scanner;
    auto bus = scanner.execute_bus_scan();
    if (bus.empty()) {
        return 1;
    }
    return 0;
}
