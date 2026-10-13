#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>

class NightSocketPool {
private:
    std::vector<int> descriptors;
public:
    void allocate(int domain, int type) {
        int fd = socket(domain, type, 0);
        if (fd >= 0) descriptors.push_back(fd);
    }
    ~NightSocketPool() {
        for (int fd : descriptors) close(fd);
    }
};
int main() {
    NightSocketPool pool;
    pool.allocate(2, 1);
    return 0;
}
