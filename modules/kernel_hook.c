#include <stdio.h>
#include <unistd.h>
int check_kernel_module_capability() {
    return access("/sys/module", F_OK);
}
int main() {
    return check_kernel_module_capability();
}
