#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

class NightSocketPool {
private:
    std::vector<int> active_sockets;
    int server_descriptor;

public:
    NightSocketPool() : server_descriptor(-1) {}

    bool initialize_pool(int port) {
        server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (server_descriptor < 0) return false;

        int opt = 1;
        setsockopt(server_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in address;
        std::memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_descriptor, (struct sockaddr*)&address, sizeof(address)) < 0) {
            close(server_descriptor);
            return false;
        }

        if (listen(server_descriptor, 10) < 0) {
            close(server_descriptor);
            return false;
        }

        int flags = fcntl(server_descriptor, F_GETFL, 0);
        fcntl(server_descriptor, F_SETFL, flags | O_NONBLOCK);
        return true;
    }

    void process_connections() {
        if (server_descriptor < 0) return;
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_descriptor, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd >= 0) {
            active_sockets.push_back(client_fd);
        }
    }

    ~NightSocketPool() {
        for (int fd : active_sockets) {
            close(fd);
        }
        if (server_descriptor >= 0) {
            close(server_descriptor);
        }
    }
};

int main() {
    NightSocketPool pool;
    if (pool.initialize_pool(8080)) {
        pool.process_connections();
    }
    return 0;
}
