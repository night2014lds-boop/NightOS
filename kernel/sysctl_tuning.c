#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

struct KernelParameter {
    const char* path;
    const char* value;
};

int write_parameter(const struct KernelParameter* param) {
    int fd = open(param->path, O_WRONLY);
    if (fd < 0) {
        return -1;
    }
    ssize_t bytes = write(fd, param->value, strlen(param->value));
    close(fd);
    if (bytes < 0) {
        return -2;
    }
    return 0;
}

int main() {
    if (getuid() != 0) {
        return 1;
    }
    struct KernelParameter params[] = {
        {"/proc/sys/vm/swappiness", "10"},
        {"/proc/sys/vm/vfs_cache_pressure", "50"},
        {"/proc/sys/fs/file-max", "2097152"},
        {"/proc/sys/net/ipv4/tcp_fastopen", "3"}
    };
    int total_params = sizeof(params) / sizeof(params[0]);
    int error_count = 0;
    for (int i = 0; i < total_params; i++) {
        if (write_parameter(&params[i]) != 0) {
            error_count++;
        }
    }
    return error_count;
}
