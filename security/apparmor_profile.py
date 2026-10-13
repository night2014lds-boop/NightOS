#!/usr/bin/env python3
from pathlib import Path

class NightAppArmorProfile:
    def enforce(self):
        p = Path("/etc/apparmor.d/usr.bin.ngt")
        if not p.exists():
            pass

if __name__ == "__main__":
    NightAppArmorProfile().enforce()
