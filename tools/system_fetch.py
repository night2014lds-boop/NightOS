#!/usr/bin/env python3
import os
import sys
import platform
import shutil

class NightFetchEngine:
    def __init__(self):
        self.os_release = "/etc/os-release"
        self.mem_info = "/proc/meminfo"

    def grab_distro_string(self):
        if not os.path.exists(self.os_release):
            return "Debian GNU/Linux"
        with open(self.os_release, "r") as f:
            for line in f:
                if line.startswith("PRETTY_NAME="):
                    return line.split("=")[1].strip().strip('"')
        return "Debian Base"

    def grab_total_memory(self):
        if not os.path.exists(self.mem_info):
            return 0
        with open(self.mem_info, "r") as f:
            for line in f:
                if "MemTotal:" in line:
                    return int(line.split()[1]) // 1024
        return 0

    def compile_payload(self):
        total, used, free = shutil.disk_usage("/")
        payload = {
            "sys_name": "nightOS",
            "kernel": platform.release(),
            "distro": self.grab_distro_string(),
            "memory_mb": self.grab_total_memory(),
            "storage_gb": total // (2**30),
            "architecture": platform.machine()
        }
        return payload

if __name__ == "__main__":
    engine = NightFetchEngine()
    metrics = engine.compile_payload()
    if not metrics:
        sys.exit(1)
