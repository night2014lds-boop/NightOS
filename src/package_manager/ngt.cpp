#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>

class NightPackageManager {
private:
    std::string base_executor;

public:
    NightPackageManager() {
        base_executor = "apt-get";
    }

    bool verify_root_privileges() {
        return getuid() == 0;
    }

    int execute_transaction(const std::string& action, const std::vector<std::string>& packages) {
        if (!verify_root_privileges()) {
            return 1;
        }
        std::string command = base_executor + " " + action + " -y ";
        for (const auto& pkg : packages) {
            command += pkg + " ";
        }
        return std::system(command.c_str());
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    std::string operation = argv[1];
    std::string mapped_action = "";
    if (operation == "install") {
        mapped_action = "install";
    } else if (operation == "remove") {
        mapped_action = "remove";
    } else if (operation == "update") {
        mapped_action = "update";
    } else if (operation == "upgrade") {
        mapped_action = "upgrade";
    } else {
        return 1;
    }
    std::vector<std::string> targets;
    for (int i = 2; i < argc; ++i) {
        targets.push_back(argv[i]);
    }
    NightPackageManager manager;
    return manager.execute_transaction(mapped_action, targets);
}
