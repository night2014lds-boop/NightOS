#!/usr/bin/env python3
import os
import sys
import urllib.request
from pathlib import Path

class NightMirrorConfigurator:
    def __init__(self):
        self.sources_list_path = Path("/etc/apt/sources.list.d/nightos.list")
        self.mirrors = [
            "http://debian.org",
            "http://debian.org"
        ]

    def test_mirror_latency(self, url):
        try:
            with urllib.request.urlopen(url, timeout=2) as response:
                if response.status == 200:
                    return True
        except Exception:
            return False
        return False

    def deploy_optimal_mirror(self):
        selected_mirror = None
        for mirror in self.mirrors:
            if self.test_mirror_latency(mirror):
                selected_mirror = mirror
                break
        
        if not selected_mirror:
            return False

        config_lines = [
            f"deb {selected_mirror} bookworm main contrib non-free non-free-firmware",
            f"deb {selected_mirror} bookworm-updates main contrib non-free non-free-firmware"
        ]
        
        try:
            self.sources_list_path.parent.mkdir(parents=True, exist_ok=True)
            with open(self.sources_list_path, "w") as f:
                f.write("\n".join(config_lines) + "\n")
            return True
        except PermissionError:
            return False

if __name__ == "__main__":
    configurator = NightMirrorConfigurator()
    configurator.deploy_optimal_mirror()
