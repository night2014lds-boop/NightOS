#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

class NightBootstrapEngine {
private:
    struct MountPoint {
        std::string source;
        std::string target;
        std::string fstype;
        unsigned long flags;
    };
    std::vector<MountPoint> vfs_table;

public:
    NightBootstrapEngine() {
        vfs_table = {
            {"proc", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC},
            {"sysfs", "/sys", "sysfs", MS_NOSUID | MS_NODEV | MS_NOEXEC},
            {"devtmpfs", "/dev", "devtmpfs", MS_NOSUID | MS_NOEXEC}
        };
    }

    bool verify_directory(const std::string& path) {
        struct stat st;
        if (stat(path.c_str(), &st) != 0) {
            return mkdir(path.c_str(), 0755) == 0;
        }
        return S_ISDIR(st.st_mode);
    }

    int execute_vfs_layer() {
        if (getuid() != 0) {
            return 1;
        }
        int failures = 0;
        for (const auto& mnt : vfs_table) {
            if (!verify_directory(mnt.target)) {
                failures++;
                continue;
            }
            if (mount(mnt.source.c_str(), mnt.target.c_str(), mnt.fstype.c_str(), mnt.flags, nullptr) != 0) {
                if (errno != EBUSY) {
                    failures++;
                }
            }
        }
        return failures;
    }
};

int main() {
    NightBootstrapEngine engine;
    return engine.execute_vfs_layer();
}
