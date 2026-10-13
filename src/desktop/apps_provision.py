#!/usr/bin/env python3
import os
import sys
import subprocess

class NightAppProvisioner:
    def __init__(self):
        self.essential_apps = [
            "xfce4-terminal",
            "thunar",
            "mousepad",
            "ristretto",
            "xfce4-taskmanager",
            "firefox-esr",
            "network-manager-gnome",
            "pavucontrol"
        ]

    def verify_and_sync(self):
        missing_apps = []
        for app in self.essential_apps:
            res = subprocess.run(["command", "-v", app], shell=True, stdout=subprocess.DEVNULL)
            if res.returncode != 0:
                missing_apps.append(app)
        
        if missing_apps:
            cmd = ["sudo", "apt-get", "install", "-y", "--no-install-recommends"] + missing_apps
            subprocess.run(cmd, stdout=subprocess.DEVNULL)

if __name__ == "__main__":
    provisioner = NightAppProvisioner()
    provisioner.verify_and_sync()
