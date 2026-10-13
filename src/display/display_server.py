#!/usr/bin/env python3
import os
import sys
import subprocess
import json
from pathlib import Path

class NightDisplayController:
    def __init__(self):
        self.xorg_conf_path = Path("/etc/X11/xorg.conf")
        self.lightdm_conf = Path("/etc/lightdm/lightdm.conf")

    def read_display_topology(self):
        try:
            res = subprocess.run(["xrandr", "--query"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            connected_displays = []
            for line in res.stdout.split("\n"):
                if " connected " in line:
                    parts = line.split()
                    display_name = parts[0]
                    geometry = parts[2] if "primary" not in parts else parts[3]
                    connected_displays.append({"name": display_name, "geometry": geometry})
            return connected_displays
        except FileNotFoundError:
            return []

    def set_display_resolution(self, output_name, resolution):
        try:
            subprocess.run(["xrandr", "--output", output_name, "--mode", resolution], check=True)
            return True
        except subprocess.CalledProcessError:
            return False

    def check_greeter_integrity(self):
        if not self.lightdm_conf.exists():
            return False
        try:
            with open(self.lightdm_conf, "r") as f:
                content = f.read()
                return "greeter-session=lightdm-gtk-greeter" in content
        except IOError:
            return False

    def execute_display_pipeline(self):
        print("--- nightOS Display Management Subsystem ---")
        displays = self.read_display_topology()
        print(f" Detected Video Output Monitors: {len(displays)}")
        for d in displays:
            print(f"  - Monitor Terminal: {d['name']} Configuration Resolution: {d['geometry']}")
        greeter_ok = self.check_greeter_integrity()
        print(f" Display Server Engine Target: LightDM DM Ready Status: {greeter_ok}")

if __name__ == "__main__":
    controller = NightDisplayController()
    controller.execute_display_pipeline()
