#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static volatile int running = 1;

void handle_signal(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        running = 0;
    }
}

int spawn_daemon_worker() {
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid > 0) exit(0);
    if (setsid() < 0) return -1;

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    pid = fork();
    if (pid < 0) return -1;
    if (pid > 0) exit(0);

    umask(0);
    if (chdir("/") < 0) return -1;

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int dev_null = open("/dev/null", O_RDWR);
    dup2(dev_null, STDIN_FILENO);
    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);

    while (running) {
        sleep(5);
    }

    close(dev_null);
    return 0;
}

int main() {
    return spawn_daemon_worker();
}
