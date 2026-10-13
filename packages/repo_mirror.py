#!/usr/bin/env python3
import urllib.request

class NightMirrorValidator:
    def verify(self):
        try:
            res = urllib.request.urlopen("http://debian.org", timeout=3)
            return res.status == 200
        except Exception:
            return False

if __name__ == "__main__":
    NightMirrorValidator().verify()
