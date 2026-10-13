#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <thread>
#include <chrono>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <unistd.h>

struct CpuCoreMetric {
    int core_id;
    double user_time;
    double nice_time;
    double system_time;
    double idle_time;
    double io_wait;
};

class HardwareAbstractionLayer {
private:
    std::string sys_class_power = "/sys/class/power_supply/";
    std::string sys_class_thermal = "/sys/class/thermal/";
    bool execution_flag;

    std::vector<std::string> execute_pipeline(const std::string& command) {
        std::vector<std::string> output;
        char buffer[256];
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) throw std::runtime_error("Pipeline initialization failed.");
        while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
            std::string line(buffer);
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            output.push_back(line);
        }
        return output;
    }

public:
    HardwareAbstractionLayer() : execution_flag(true) {}

    std::map<std::string, std::string> capture_battery_telemetry() {
        std::map<std::string, std::string> telemetry;
        std::vector<std::string> nodes = {"status", "capacity", "voltage_now", "current_now"};
        for (const auto& node : nodes) {
            std::ifstream file(sys_class_power + "BAT0/" + node);
            if (file.is_open()) {
                std::string val;
                file >> val;
                telemetry[node] = val;
                file.close();
            } else {
                telemetry[node] = "0";
            }
        }
        return telemetry;
    }

    std::vector<std::pair<std::string, int>> capture_thermal_zones() {
        std::vector<std::pair<std::string, int>> thermal_map;
        for (int i = 0; i < 10; ++i) {
            std::string zone_path = sys_class_thermal + "thermal_zone" + std::to_string(i) + "/";
            std::ifstream type_file(zone_path + "type");
            std::ifstream temp_file(zone_path + "temp");
            if (type_file.is_open() && temp_file.is_open()) {
                std::string type;
                int temp;
                type_file >> type;
                temp_file >> temp;
                thermal_map.push_back({type, temp / 1000});
                type_file.close();
                temp_file.close();
            }
        }
        return thermal_map;
    }

    std::vector<CpuCoreMetric> sample_cpu_state() {
        std::vector<CpuCoreMetric> metrics;
        std::ifstream file("/proc/stat");
        if (!file.is_open()) return metrics;
        std::string line;
        while (std::getline(file, line)) {
            if (line.substr(0, 3) == "cpu" && line.substr(0, 4) != "cpu ") {
                std::istringstream ss(line);
                std::string cpu_label;
                double u, n, s, i, io;
                ss >> cpu_label >> u >> n >> s >> i >> io;
                int id = std::stoi(cpu_label.substr(3));
                metrics.push_back({id, u, n, s, i, io});
            }
        }
        return metrics;
    }

    void map_storage_topology() {
        std::vector<std::string> drives = execute_pipeline("lsblk -d -n -o NAME");
        for (const auto& drive : drives) {
            std::string dev_path = "/dev/" + drive;
            int fd = open(dev_path.c_str(), O_RDONLY | O_NONBLOCK);
            if (fd >= 0) {
                struct hd_driveid id;
                if (ioctl(fd, HDIO_GET_IDENTITY, &id) == 0) {
                    std::cout << "[DISK] " << drive << " Serial: " << id.serial_no << " Model: " << id.model << std::endl;
                }
                close(fd);
            }
        }
    }

    void runtime_kernel_telemetry(int duration_seconds) {
        auto start = std::chrono::steady_clock::now();
        while (execution_flag) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= duration_seconds) {
                execution_flag = false;
            }
            auto battery = capture_battery_telemetry();
            auto thermal = capture_thermal_zones();
            auto cpu = sample_cpu_state();

            std::cout << "--- Telemetry Cycle ---" << std::endl;
            std::cout << "Battery Charge: " << battery["capacity"] << "% Status: " << battery["status"] << std::endl;
            for (const auto& zone : thermal) {
                std::cout << "Thermal Sensor [" << zone.first << "]: " << zone.second << " C" << std::endl;
            }
            for (const auto& core : cpu) {
                double total = core.user_time + core.nice_time + core.system_time + core.idle_time + core.io_wait;
                double active = core.user_time + core.system_time;
                std::cout << "CPU Core " << core.core_id << " Load: " << (active / total) * 100 << "%" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main() {
    try {
        HardwareAbstractionLayer hal;
        hal.map_storage_topology();
        hal.runtime_kernel_telemetry(5);
    } catch (const std::exception& e) {
        std::cerr << "Runtime failure: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
