#!/usr/bin/env python3
import os
import subprocess

class NightBuildPipeline:
    def execute(self):
        if os.path.exists("bin/binary_stub.c"):
            subprocess.run(["gcc", "bin/binary_stub.c", "-o", "build/stub_bin"])

if __name__ == "__main__":
    NightBuildPipeline().execute()
