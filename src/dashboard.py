#!/usr/bin/env python3
import sys
import os
import subprocess
import shutil

class NightDashboard:
    def __init__(self):
        self.critical_services = ["lightdm", "NetworkManager", "ufw", "cron"]

    def get_service_status(self):
        status_dict = {}
        for service in self.critical_services:
            proc = subprocess.run(["systemctl", "is-active", service], stdout=subprocess.PIPE, text=True)
            status_dict[service] = proc.stdout.strip()
        return status_dict

    def get_storage_info(self):
        total, used, free = shutil.disk_usage("/")
        return {
            "total": total // (2**30),
            "used": used // (2**30),
            "free": free // (2**30)
        }

    def display_metrics(self):
        print("====================================")
        print("      nightOS SYSTEM DASHBOARD      ")
        print("====================================")
        print("\n[Service Monitor]")
        for svc, state in self.get_service_status().items():
            print(f"  {svc.ljust(16)} : {state.upper()}")
        
        storage = self.get_storage_info()
        print("\n[Storage Monitor]")
        print(f"  Total Space     : {storage['total']} GB")
        print(f"  Used Space      : {storage['used']} GB")
        print(f"  Available Space : {storage['free']} GB")
        print("====================================")

    def manage_network(self, action):
        if action == "up":
            subprocess.run(["nmcli", "networking", "on"])
        elif action == "off":
            subprocess.run(["nmcli", "networking", "off"])

    def trigger_power(self, mode):
        if mode == "reboot":
            os.system("systemctl reboot")
        elif mode == "poweroff":
            os.system("systemctl poweroff")

if __name__ == "__main__":
    dash = NightDashboard()
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        if arg == "--network-on":
            dash.manage_network("up")
        elif arg == "--network-off":
            dash.manage_network("off")
        elif arg == "--reboot":
            dash.trigger_power("reboot")
        elif arg == "--poweroff":
            dash.trigger_power("poweroff")
    else:
        dash.display_metrics()
