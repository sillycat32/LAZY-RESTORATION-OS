# Zero-Latency Application Switcher

## Overview

The Zero-Latency Application Switcher is a user-space operating systems project that demonstrates how application resume latency can be reduced using snapshot-based hibernation and lazy restoration. Instead of fully restarting applications, the system freezes a running application, captures minimal snapshot metadata, terminates it to reclaim resources, and restores it on demand using different strategies.

The project provides an interactive terminal-based dashboard built using ncurses to control snapshot and restore operations and visualize latency comparisons between cold start, eager restore, and lazy restore.

---

## Key Features

- User-space snapshot and hibernation of applications
- Support for lazy restore, eager restore, and cold start
- Accurate latency measurement using CLOCK_MONOTONIC
- Interactive ncurses-based dashboard
- ASCII bar graph visualization of performance
- Application-agnostic (supports GUI apps like xclock, xcalc, xeyes)

---

## System Requirements

- Linux-based operating system
- GCC compiler
- ncurses development library
- X11 environment (for GUI applications)

Install dependencies (Ubuntu/Debian):
```bash
sudo apt install build-essential libncurses-dev x11-apps
