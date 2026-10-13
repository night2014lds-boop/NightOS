#!/usr/bin/env python3
import os
import sys
import subprocess
from pathlib import Path

class NightSecurityPolicy:
    def __init__(self):
        self.policy_dir = Path("/etc/apparmor.d")
        self.target_binary = "/usr/bin/ngt"

    def generate_strict_profile(self):
        profile_content = f"""
{self.target_binary} {{
  file,
  capability sys_admin,
  capability sys_rawio,
  /bin/** r,
  /usr/bin/** r,
  /etc/apt/** rw,
  /var/lib/dpkg/** rwk,
  /var/cache/apt/** rw,
}}
"""
        profile_path = self.policy_dir / "usr.bin.ngt"
        try:
            with open(profile_path, "w") as f:
                f.write(profile_content.strip())
            return True
        except PermissionError:
            return False

    def load_policy_context(self):
        if not os.path.exists("/lib/apparmor/functions"):
            return False
        try:
            subprocess.run(["apparmor_parser", "-r", str(self.policy_dir / "usr.bin.ngt")], check=True)
            return True
        except subprocess.CalledProcessError:
            return False

if __name__ == "__main__":
    policy = NightSecurityPolicy()
    if os.getuid() == 0:
        policy.generate_strict_profile()
        policy.load_policy_context()
