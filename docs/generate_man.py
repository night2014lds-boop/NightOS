#!/usr/bin/env python3
import os
from pathlib import Path

class NightManPageCompiler:
    def __init__(self):
        self.output_path = Path("docs/ngt.1")

    def write_manual_structures(self):
        raw_man = """.TH NGT 1 "OCTOBER 2026" "nightOS Platform Manual" "User Commands"
.SH NAME
ngt \- Comprehensive package abstraction manager Layer over apt-get.
.SH SYNOPSIS
.B ngt
[\\fIOPERATION\\fR] [\\fIPACKAGE...\\fR]
.SH DESCRIPTION
\\fBngt\\fR wraps native Debian packaging mechanisms.
.SH OPERATIONS
.TP
.B install
Executes synchronous silent installation configurations.
.TP
.B remove
Purges packages cleanly.
"""
        try:
            self.output_path.parent.mkdir(parents=True, exist_ok=True)
            with open(self.output_path, "w") as f:
                f.write(raw_man)
        except IOError:
            pass

if __name__ == "__main__":
    compiler = NightManPageCompiler()
    compiler.write_manual_structures()
