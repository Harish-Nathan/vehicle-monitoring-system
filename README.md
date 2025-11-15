# Multithreaded Vehicle Monitoring System

**Real-time Linux system programming project** that simulates vehicle sensors (speed, fuel, temperature) and demonstrates production-style logging, thread-safe shared state, hysteresis/debounce logic, and a live terminal UI.

---

## Overview
This project is a compact, runnable example of system-level C programming on Linux. Each sensor runs in its own POSIX thread; shared state is protected with mutexes and condition variables. The system logs JSON telemetry to `logs/vehicle.log`, raises event alerts based on hysteresis logic, and provides a live ncurses-based dashboard (`ui`) for real-time visualization. The program supports graceful shutdown via `SIGINT`.

---

## Features
- Multithreaded sensor simulation (speed, fuel, temperature)  
- POSIX threads, mutex + condvar synchronization  
- Hysteresis-based alert thresholds and debounce logic  
- JSON formatted telemetry logging (`logs/vehicle.log`)  
- ncurses-based live dashboard for visualization (`ui.c`)  
- Clean SIGINT-driven shutdown that flushes logs and joins threads

---

## Repo layout

Makefile
README.md
include/
└─ data.h # shared data structures and constants
logs/
└─ vehicle.log # runtime telemetry logs (gitignored)
src/
├─ main.c # program entry, init, signal handling
├─ speed.c # speed sensor thread & logic
├─ fuel.c # fuel sensor thread & logic
├─ temp.c # temperature sensor thread & logic
├─ logger.c # JSON logging utilities
├─ utils.c # helper utilities (timestamp, json)
└─ ui.c # ncurses dashboard & rendering
vehicle_monitor # binary (not checked into git)


---

## Build & Run

### Prerequisites
- Linux (Ubuntu recommended)  
- `gcc`, `make`  
- `libncurses` development headers (`sudo apt install libncurses5-dev libncursesw5-dev`)  
- `jq` (optional, for pretty-printing logs during dev)

### Build
```bash
make

Run
./vehicle_monitor


Press Ctrl+C to exit cleanly.

Sample log entry

logs/vehicle.log

{"timestamp":"2025-11-12 14:22:01","sensor":"speed","event":"SPEED_HIGH","value":102}

File responsibilities

src/main.c — initializes system, spawns threads, handles signals, joins threads on exit.

src/speed.c, src/fuel.c, src/temp.c — simulate sensor readings, apply hysteresis/debounce and update shared state.

src/logger.c — thread-safe JSON logging functions and log rotation helper (if any).

src/ui.c — ncurses-based dashboard showing current values and alerts.

src/utils.c — timestamp formatting, small helpers (safe string ops, config loader).

include/data.h — shared data structure definitions (sensor struct, mutex/cond, thresholds).

Development notes & next steps

Add configuration file (cfg/thresholds.json) for runtime tuning.

Replace sensor simulation with live input (socket or serial) for hardware integration.

Add unit tests for hysteresis and debounce logic.

Add log rotation and size limits for long-running tests.

Author

Harish S — Embedded Linux & System Programming
LinkedIn: https://www.linkedin.com/in/harish-s-embedded/

Email: harishnathan024@gmail.com

