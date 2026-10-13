#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <unistd.h>

class NightProcessReaper {
public:
    void audit_running_process_tree() {
        std::cout << "[PROCESS] Scanning local execution tables..." << std::endl;
        DIR* dir = opendir("/proc");
        if (!dir) return;

        struct dirent* entry;
        int active_pids = 0;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                std::string name = entry->d_name;
                if (name.find_first_not_of("0123456789") == std::string::npos) {
                    active_pids++;
                }
            }
        }
        closedir(dir);
        std::cout << "  - Active unique structural thread processes found: " << active_pids << std::endl;
    }
};

int main() {
    NightProcessReaper reaper;
    reaper.audit_running_process_tree();
    return 0;
}
