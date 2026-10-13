#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>

class NightInstaller {
private:
    std::vector<std::string> base_packages;
    std::vector<std::string> xfce_packages;

public:
    NightInstaller() {
        base_packages = {"sudo", "curl", "wget", "git", "micro", "fastfetch", "htop", "ufw"};
        xfce_packages = {"xfce4", "xfce4-goodies", "lightdm", "lightdm-gtk-greeter"};
    }

    bool check_root() {
        return getuid() == 0;
    }

    void prepare_repositories() {
        std::ofstream sources("/etc/apt/sources.list", std::ios::trunc);
        if (sources.is_open()) {
            sources << "deb http://debian.org bookworm main contrib non-free non-free-firmware\n";
            sources << "deb http://debian.org bookworm-updates main contrib non-free non-free-firmware\n";
            sources << "deb http://debian.org bookworm-security main contrib non-free non-free-firmware\n";
            sources.close();
        }
        std::system("apt-get update -y");
    }

    void install_package_group(const std::vector<std::string>& packages) {
        std::string cmd = "apt-get install -y --no-install-recommends ";
        for (const auto& pkg : packages) {
            cmd += pkg + " ";
        }
        std::system(cmd.c_str());
    }

    void configure_system() {
        std::system("systemctl enable lightdm");
        std::system("systemctl enable ufw");
        std::system("ufw default deny incoming");
        std::system("ufw default allow outgoing");
        std::system("ufw enable");
        std::system("apt-get autoremove -y && apt-get clean");
    }

    void execute() {
        if (!check_root()) {
            std::cerr << "Root privileges required." << std::endl;
            std::exit(1);
        }
        prepare_repositories();
        install_package_group(base_packages);
        install_package_group(xfce_packages);
        configure_system();
        std::cout << "nightOS base installation complete finished." << std::endl;
    }
};

int main() {
    NightInstaller installer;
    installer.execute();
    return 0;
}
