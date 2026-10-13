#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void transform_to_daemon() {
    pid_t pid = fork();
    if (pid > 0) _exit(0);
    if (pid < 0) _exit(1);
    if (setsid() < 0) _exit(1);
    umask(0);
    chdir("/");
    close(0); close(1); close(2);
}

int main() {
    transform_to_daemon();
    while (1) {
        sleep(60);
    }
    return 0;
}
