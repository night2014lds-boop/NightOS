#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

class NightSharedLibrary {
public:
    int attach_segment(int key, size_t size) {
        int id = shmget(key, size, 0666);
        return id;
    }
};
