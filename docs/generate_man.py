#!/usr/bin/env python3
from pathlib import Path

class NightManGenerator:
    def build(self):
        content = ".TH NGT 1\n.SH NAME\nngt \\- nightOS package manager"
        Path("docs/ngt.1").write_text(content)

if __name__ == "__main__":
    NightManGenerator().build()
