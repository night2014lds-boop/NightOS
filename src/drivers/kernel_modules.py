#!/usr/bin/env python3
import os
import sys

class NightKernelDriverManager:
    def __init__(self):
        self.proc_modules = "/proc/modules"

    def read_loaded_drivers(self):
        print("[DRIVERS] Auditing Linux Kernel Hardware Modules...")
        if not os.path.exists(self.proc_modules):
            print("  - Infrastructure pipeline unavailable.")
            return
        
        with open(self.proc_modules, "r") as f:
            for line in f.readlines()[:15]:
                parts = line.split()
                module_name = parts[0]
                memory_size = parts[1]
                instances = parts[2]
                print(f"  - Module Object: {module_name.ljust(20)} | Volume Allocation: {memory_size.rjust(8)} Bytes | Active Links: {instances}")

if __name__ == "__main__":
    driver_sys = NightKernelDriverManager()
    driver_sys.read_loaded_drivers()
