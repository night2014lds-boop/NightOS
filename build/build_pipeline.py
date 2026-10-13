#!/usr/bin/env python3
import os
import sys
import subprocess
from pathlib import Path

class NightBuildPipeline:
    def __init__(self):
        self.workspace = Path(".")
        self.output_bin_dir = Path("build/output")
        self.compiler_flags = ["-O3", "-Wall", "-std=c++17"]

    def setup_environment(self):
        self.output_bin_dir.mkdir(parents=True, exist_ok=True)

    def compile_cpp_module(self, source_path, binary_name):
        if not os.path.exists(source_path):
            return False
        output_path = self.output_bin_dir / binary_name
        cmd = ["g++"] + self.compiler_flags + [str(source_path), "-o", str(output_path)]
        try:
            res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
            return res.returncode == 0
        except subprocess.CalledProcessError:
            return False

    def scan_and_build(self):
        self.setup_environment()
        cpp_targets = {
            "init/system_bootstrap.cpp": "system_bootstrap",
            "drivers/bus_pci.cpp": "bus_pci",
            "network/socket_pool.cpp": "socket_pool",
            "lib/shared_memory_layer.cpp": "shared_memory_layer"
        }
        success = True
        for src, bin_name in cpp_targets.items():
            if not self.compile_cpp_module(src, bin_name):
                success = False
        return success

if __name__ == "__main__":
    pipeline = NightBuildPipeline()
    if not pipeline.scan_and_build():
        sys.exit(1)
