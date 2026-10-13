#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>
#include <mntent.h>

void analyze_filesystem_topology() {
    struct mntent *ent;
    FILE *aFile = setmntent("/proc/mounts", "r");
    if (aFile == NULL) {
        perror("Could not read mounting structures.");
        exit(1);
    }

    printf("--- nightOS Filesystem Integrity Mount Matrix ---\n");
    while ((ent = getmntent(aFile)) != NULL) {
        if (strncmp(ent->mnt_fsname, "/dev/", 5) == 0) {
            struct statfs stats;
            if (statfs(ent->mnt_dir, &stats) == 0) {
                unsigned long long total_blocks = stats.f_blocks * stats.f_bsize;
                unsigned long long free_blocks = stats.f_bfree * stats.f_bsize;
                unsigned long long used_blocks = total_blocks - free_blocks;
                
                printf(" Device Storage Block: %s\n", ent->mnt_fsname);
                printf("  Mounted Endpoint Target: %s\n", ent->mnt_dir);
                printf("  VFS Filesystem Engine: %s\n", ent->mnt_type);
                printf("  Volumetric Size Capacity: %llu MB | Free Space Space: %llu MB\n", 
                       total_blocks / (1024 * 1024), free_blocks / (1024 * 1024));
            }
        }
    }
    endmntent(aFile);
}

int main() {
    analyze_filesystem_topology();
    return 0;
}
