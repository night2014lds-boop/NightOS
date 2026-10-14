# nightOS — Monolithic Operating System Architecture

nightOS is a high-performance, modular operating system abstraction built directly on top of the rock-solid Debian GNU/Linux core ecosystem. The platform enforces maximum hardware optimization by stripping downstream bloat and shipping with a pure, vanilla XFCE desktop environment layer integrated alongside low-level system daemons developed in C++, C, and Python.

## Technical Infrastructure

* **Base Layer**: Debian Linux Core (Bookworm Architecture)
* **Desktop Environment**: Pure Vanilla XFCE for ultra-low memory footprints and standard pointer precision.
* **Core Languages**: Native C++17 (System engines), ISO C99 (Kernel tuning & IO), Python 3.11+ (Automation & Daemons).
* **Package Abstraction**: Proprietary `ngt` package manager wrapping apt transactions natively.

---

## Repository Directory Blueprint

This project enforces strict organizational boundaries across 16 modular root directories:

### Core Frameworks
* **`api/`**: Houses native low-latency REST endpoints running on multi-threaded TCP/IP channels for structural network querying.
* **`bin/`**: Dedicated target directory for static core binaries and standalone system utilities.
* **`build/`**: Automated compiling workspace driving the high-speed modular build pipeline system.
* **`docs/`**: Holds official system documentation manuals and compressed Linux man-pages (`ngt.1`).
* **`include/`**: C/C++ architecture header definitions (`nightos_core.h`) declaring global runtime operational hooks.
* **`init/`**: Multi-threaded Virtual Filesystem (VFS) bootstrap routine responsible for preparing `/proc`, `/sys`, and `/dev`.
* **`kernel/`**: Monolithic configuration controller tuning direct sysctl memory footprints and virtual memory page swappiness.
* **`lib/`**: High-performance Inter-Process Communication (IPC) library managing low-level dynamic shared memory segments.
* **`modules/`**: Contains runtime kernel compatibility checkers and validation engines.
* **`src/`**: Central operational pipeline hosting active modules for storage IO blocks, network interface layers, and session launching.
* **`tests/`**: Unit testing runner framework executing validation assertions across active architectural components.
* **`tools/`**: Automated information fetch script reading kernel signatures and system metrics instantly.

### Distribution Components
* **`assets/`**: Standard theme manifests defining vanilla XFCE appearance schemas, cursors, and icon topologies.
* **`drivers/`**: Hardware PCI bus interrogation node translating device identities from raw sysfs nodes.
* **`network/`**: Implements non-blocking concurrent socket connection pools for stable system communications.
* **`packages/`**: Hosts mirror verification tools evaluating live network package server latencies.
* **`scripts/`**: Automation layer orchestrating image creation frameworks via live-build and debootstrap.

---

## Binary Production Pipeline

### Building From Source
To compile all native components and synchronize permissions across the execution tree, trigger the integrated automated optimizer tool:
```bash
python3 organizer.py
./build/build_pipeline.py
```

### Compiling the Live Installation ISO
To package the complete Debian kernel space and default XFCE workspace into a deployable distribution medium:
```bash
sudo ./scripts/build.sh
```

---

## Official Installation Releases

If you want to evaluate nightOS immediately without compiling the monolithic repository tree manually, you can acquire pre-built binary distributions directly from the platform hosting framework.

### How to Install the System Release
1. Navigate to the right-hand panel of this page and locate the **Releases** section.
2. Click on the latest stable version identifier tag (e.g., `v1.0.0`).
3. Under the **Assets** dropdown menu, download the official installation image file (compiled as a standard `.iso` or `.img` container file).
4. Flash the downloaded image block onto a physical USB storage drive utilizing a raw disk-writing terminal pipeline:
```bash
sudo dd if=nightos_v1.0.0.iso of=/dev/sdX bs=4M status=progress conv=fdatasync
```
5. Plug the storage drive into your host machine terminal, reboot into the system BIOS firmware configuration, select the medium as the primary boot target, and launch the standalone installer wrapper.
