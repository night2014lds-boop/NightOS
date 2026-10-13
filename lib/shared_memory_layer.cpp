#include <iostream>
#include <string>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

class NightSharedMemoryLayer {
private:
    int segment_id;
    key_t segment_key;
    size_t buffer_size;
    void* attached_memory;

public:
    NightSharedMemoryLayer(int token_id, size_t size) : buffer_size(size), attached_memory(nullptr) {
        segment_key = ftok("/tmp", token_id);
    }

    bool allocate_segment() {
        segment_id = shmget(segment_key, buffer_size, IPC_CREAT | 0666);
        if (segment_id < 0) return false;
        attached_memory = shmat(segment_id, nullptr, 0);
        return attached_memory != (void*)-1;
    }

    bool write_shared_data(const char* data, size_t length) {
        if (!attached_memory || length > buffer_size) return false;
        std::memcpy(attached_memory, data, length);
        return true;
    }

    bool read_shared_data(char* output_buffer, size_t length) {
        if (!attached_memory || length > buffer_size) return false;
        std::memcpy(output_buffer, attached_memory, length);
        return true;
    }

    void detach_and_cleanup() {
        if (attached_memory && attached_memory != (void*)-1) {
            shmdt(attached_memory);
        }
        if (segment_id >= 0) {
            shmctl(segment_id, IPC_RMID, nullptr);
        }
    }

    ~NightSharedMemoryLayer() {
        detach_and_cleanup();
    }
};

int main() {
    NightSharedMemoryLayer memory_layer(99, 4096);
    if (memory_layer.allocate_segment()) {
        char buffer[32] = "NIGHTOS_MEMORY_SHARED_STATE";
        memory_layer.write_shared_data(buffer, 32);
        char reader[32];
        memory_layer.read_shared_data(reader, 32);
    }
    return 0;
}
