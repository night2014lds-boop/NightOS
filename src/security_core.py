#!/usr/bin/env python3
import os
import sys
import hashlib
import secrets
import json
import base64
from pathlib import Path
import subprocess

class NightSecurityCore:
    def __init__(self):
        self.audit_log_path = Path("/var/log/nightos_security.log")
        self.signature_db = Path("/etc/nightos_signatures.json")
        self.monitored_binaries = ["/bin/bash", "/bin/ls", "/bin/login", "/sbin/init"]

    def generate_cryptographic_salt(self, length=32):
        return secrets.token_hex(length)

    def derive_system_key(self, pass_phrase, salt):
        key = hashlib.pbkdf2_hmac('sha256', pass_phrase.encode('utf-8'), salt.encode('utf-8'), 100000)
        return base64.b64encode(key).decode('utf-8')

    def calculate_file_hash(self, target_path):
        sha256_hash = hashlib.sha256()
        try:
            with open(target_path, "rb") as f:
                for byte_block in iter(lambda: f.read(65536), b""):
                    sha256_hash.update(byte_block)
            return sha256_hash.hexdigest()
        except FileNotFoundError:
            return None

    def establish_security_baseline(self):
        baseline = {}
        for binary in self.monitored_binaries:
            f_hash = self.calculate_file_hash(binary)
            if f_hash:
                baseline[binary] = f_hash
        try:
            with open(self.signature_db, "w") as f:
                json.dump(baseline, f, indent=4)
            return True
        except PermissionError:
            return False

    def integrity_verification_audit(self):
        if not self.signature_db.exists():
            return False, ["Baseline signatures database missing."]
        with open(self.signature_db, "r") as f:
            baseline = json.load(f)
        violations = []
        for binary, original_hash in baseline.items():
            current_hash = self.calculate_file_hash(binary)
            if not current_hash:
                violations.append(f"Binary removed or broken: {binary}")
            elif current_hash != original_hash:
                violations.append(f"Integrity violation detected in binary: {binary}")
        return len(violations) == 0, violations

    def audit_active_firewall(self):
        try:
            res = subprocess.run(["iptables", "-L", "-n"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            chains = [line for line in res.stdout.split("\n") if "Chain" in line]
            return chains
        except Exception:
            return []

    def execute_security_hardening(self):
        os.makedirs(self.audit_log_path.parent, exist_ok=True)
        is_secure, reports = self.integrity_verification_audit()
        print("--- nightOS Security Enforcement Subsystem ---")
        print(f"System Integrity State: {'SECURE' if is_secure else 'COMPROMISED'}")
        if not is_secure:
            for report in reports:
                print(f"  [ALERT] {report}")
        active_rules = self.audit_audit_firewall = self.audit_active_firewall()
        print(f"Firewall Rulesets Active: {len(active_rules)} network policies.")

if __name__ == "__main__":
    sec_engine = NightSecurityCore()
    if not sec_engine.signature_db.exists():
        sec_engine.establish_security_baseline()
    sec_engine.execute_security_hardening()
