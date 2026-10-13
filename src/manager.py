#!/usr/bin/env python3
import sys
import os
import subprocess
import json
import time
from pathlib import Path

class NightManager:
    def __init__(self):
        self.config_dir = Path.home() / ".config" / "nightos"
        self.services = ["lightdm", "xfce4-session", "networking", "dbus"]
        self.ensure_environment()

    def ensure_environment(self):
        self.config_dir.mkdir(parents=True, exist_ok=True)
        config_file = self.config_dir / "settings.json"
        if not config_file.exists():
            default_settings = {
                "version": "1.0.0",
                "environment": "XFCE",
                "base_distro": "Debian",
                "update_policy": "automatic"
            }
            with open(config_file, "w") as f:
                json.dump(default_settings, f, indent=4)

    def check_system_services(self):
        status_report = {}
        for service in self.services:
            try:
                result = subprocess.run(
                    ["systemctl", "is-active", service],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True
                )
                status_report[service] = result.stdout.strip()
            except Exception:
                status_report[service] = "unknown"
        return status_report

    def check_debian_updates(self):
        try:
            subprocess.run(["sudo", "apt-get", "update"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            result = subprocess.run(
                ["apt-get", "-s", "upgrade"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            upgradable = [line for line in result.stdout.split("\n") if "Inst" in line]
            return len(upgradable)
        except Exception:
            return -1

    def run_diagnostics(self):
        print("--- nightOS Diagnostic Utility ---")
        print(f"Config Directory: {self.config_dir}")
        print("\nChecking Daemon Services:")
        for svc, status in self.check_system_services().items():
            print(f"  - {svc}: {status}")
        
        print("\nChecking Package Repository:")
        updates = self.check_debian_updates()
        if updates >= 0:
            print(f"  - Pending Updates: {updates} packages can be upgraded.")
        else:
            print("  - Pending Updates: Unable to access apt infrastructure.")

if __name__ == "__main__":
    manager = NightManager()
    manager.run_diagnostics()
