#!/usr/bin/env python3
import json
from pathlib import Path

class NightThemeManifest:
    def __init__(self):
        self.target = Path("assets/manifest.json")

    def generate(self):
        data = {
            "pointer": "Adwaita",
            "icons": "Tango",
            "wm_theme": "Default",
            "font": "Sans 10"
        }
        with open(self.target, "w") as f:
            json.dump(data, f, indent=4)

if __name__ == "__main__":
    NightThemeManifest().generate()
