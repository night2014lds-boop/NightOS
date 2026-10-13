#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sys/sysinfo.h>
#include <sys/stat.h>

class NightCore {
private:
    std::string log_file;
    void write_log(const std::string& type, const std::string& message) {
        std::ofstream file(log_file, std::ios::app);
        if (file.is_open()) {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string time_str = std::ctime(&now);
            time_str.pop_back();
            file << "[" << time_str << "] [" << type << "] " << message << std::endl;
        }
    }

public:
    NightCore(std::string log_path) : log_file(log_path) {}

    void initialize_system() {
        write_log("INFO", "Initializing nightOS Core Engine...");
        std::vector<std::string> critical_paths = {"/etc", "/bin", "/var", "/usr"};
        struct stat info;
        for (const auto& path : critical_paths) {
            if (stat(path.c_str(), &info) != 0) {
                write_log("ERROR", "Critical path missing or inaccessible: " + path);
            } else {
                write_log("INFO", "Verified path: " + path);
            }
        }
    }

    void monitor_resources() {
        struct sysinfo mem_info;
        if (sysinfo(&mem_info) == 0) {
            long long total_ram = mem_info.totalram * mem_info.mem_unit;
            long long free_ram = mem_info.freeram * mem_info.mem_unit;
            long long used_ram = total_ram - free_ram;
            write_log("RESOURCE", "Total RAM: " + std::to_string(total_ram / (1024 * 1024)) + " MB");
            write_log("RESOURCE", "Used RAM: " + std::to_string(used_ram / (1024 * 1024)) + " MB");
            write_log("RESOURCE", "Free RAM: " + std::to_string(free_ram / (1024 * 1024)) + " MB");
            write_log("RESOURCE", "Active Processes: " + std::to_string(mem_info.procs));
        } else {
            write_log("ERROR", "Failed to retrieve system memory info");
        }
    }

    void shutdown_system() {
        write_log("INFO", "Shutting down nightOS Core Engine clean.");
    }
};

int main() {
    NightCore core("/var/log/nightos_core.log");
    core.initialize_system();
    core.monitor_resources();
    core.shutdown_system();
    std::cout << "nightOS Core executed diagnostics successfully." << std::endl;
    return 0;
}
