#include <stdio.h>
#include <stdlib.h>
void apply_kernel_optimizations() {
    system("sysctl -w vm.swappiness=10 > /dev/null 2>&1");
    system("sysctl -w fs.file-max=2097152 > /dev/null 2>&1");
}
int main() {
    apply_kernel_optimizations();
    return 0;
}
