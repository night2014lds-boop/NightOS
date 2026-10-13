#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>

struct InterfaceDetails {
    std::string interface_name;
    std::string ip_address;
    std::string mac_address;
    bool is_up;
};

class NightNetworkCore {
private:
    std::vector<std::string> target_dns_servers;

public:
    NightNetworkCore() {
        target_dns_servers = {"8.8.8.8", "1.1.1.1", "9.9.9.9"};
    }

    std::vector<InterfaceDetails> discover_interfaces() {
        std::vector<InterfaceDetails> interfaces;
        char buffer[1024];
        struct ifconf ifc;
        struct ifreq* ifr;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) return interfaces;

        ifc.ifc_len = sizeof(buffer);
        ifc.ifc_buf = buffer;
        if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
            close(sock);
            return interfaces;
        }

        ifr = ifc.ifc_req;
        int num_interfaces = ifc.ifc_len / sizeof(struct ifreq);

        for (int i = 0; i < num_interfaces; i++) {
            struct ifreq req = ifr[i];
            InterfaceDetails details;
            details.interface_name = req.ifr_name;

            struct ifreq flags_req;
            std::strncpy(flags_req.ifr_name, req.ifr_name, IFNAMSIZ);
            if (ioctl(sock, SIOCGIFFLAGS, &flags_req) >= 0) {
                details.is_up = (flags_req.ifr_flags & IFF_UP) != 0;
            } else {
                details.is_up = false;
            }

            struct ifreq ip_req;
            std::strncpy(ip_req.ifr_name, req.ifr_name, IFNAMSIZ);
            if (ioctl(sock, SIOCGIFADDR, &ip_req) >= 0) {
                details.ip_address = inet_ntoa(((struct sockaddr_in*)&ip_req.ifr_addr)->sin_addr);
            } else {
                details.ip_address = "0.0.0.0";
            }

            struct ifreq mac_req;
            std::strncpy(mac_req.ifr_name, req.ifr_name, IFNAMSIZ);
            if (ioctl(sock, SIOCGIFHWADDR, &mac_req) >= 0) {
                char mac_str[18];
                std::snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[0],
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[1],
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[2],
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[3],
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[4],
                    (unsigned char)mac_req.ifr_hwaddr.sa_data[5]);
                details.mac_address = mac_str;
            } else {
                details.mac_address = "00:00:00:00:00:00";
            }

            interfaces.push_back(details);
        }

        close(sock);
        return interfaces;
    }

    bool verify_dns_connectivity() {
        for (const auto& dns : target_dns_servers) {
            int sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock < 0) continue;

            struct sockaddr_in serv_addr;
            std::memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(53);
            inet_pton(AF_INET, dns.c_str(), &serv_addr.sin_addr);

            struct timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

            if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
                close(sock);
                return true;
            }
            close(sock);
        }
        return false;
    }
};

int main() {
    NightNetworkCore net_core;
    auto interfaces = net_core.discover_interfaces();
    std::cout << "[nightOS Network Architecture Initialized]" << std::endl;
    for (const auto& net_if : interfaces) {
        std::cout << " Interface: " << net_if.interface_name 
                  << " | IP: " << net_if.ip_address 
                  << " | MAC: " << net_if.mac_address 
                  << " | Status: " << (net_if.is_up ? "ACTIVE" : "INACTIVE") << std::endl;
    }
    bool online = net_core.verify_dns_connectivity();
    std::cout << " WAN Connectivity State: " << (online ? "ONLINE" : "OFFLINE") << std::endl;
    return 0;
}
