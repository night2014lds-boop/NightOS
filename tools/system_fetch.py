#!/usr/bin/env python3
import os

class NightSystemFetch:
    def output(self):
        if os.path.exists("/proc/version"):
            with open("/proc/version", "r") as f:
                return f.read()
        return ""

if __name__ == "__main__":
    NightSystemFetch().output()
