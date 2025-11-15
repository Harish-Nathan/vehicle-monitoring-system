# 🚗 Multithreaded Vehicle Monitoring System  
*A Linux system-programming project demonstrating real-time sensor simulation, POSIX threads, synchronization, logging, and a live ncurses dashboard.*

---

## 🌟 Overview

This project models a simplified vehicle ECU-style monitoring system with three independent sensors:

- **Speed**
- **Fuel Level**
- **Engine Temperature**

Each sensor runs in its own **POSIX thread** and updates a shared state protected with **mutex + condition variables**.  
The system generates **JSON-formatted logs**, applies **hysteresis and debounce** for stable alerting, and shows a **real-time dashboard** using ncurses.  
A **clean shutdown** is handled using `SIGINT`.

This project demonstrates **real embedded Linux system programming**, not classroom-level C.

---

## 🔧 Key Features

- 🧵 **Multithreaded sensor simulation** (speed, fuel, temperature)  
- 🔐 **Thread-safe shared data** using mutex and condition variables  
- 📉 **Hysteresis-based alert logic** (prevents flicker/flapping)  
- ⏳ **Debounce filter** on sensor transitions  
- 🪵 **JSON telemetry logging** (`logs/vehicle.log`)  
- 📺 **Ncurses dashboard** for real-time monitoring  
- 🧹 **Graceful SIGINT shutdown** (joins threads + flushes logs)  
- 🧩 **Modular, maintainable architecture**  

---

## 📂 Project Structure

```
vehicle-monitoring-system/
│
├── src/
│   ├── main.c          # Program entry, thread launch, SIGINT handling
│   ├── speed.c         # Speed sensor thread
│   ├── fuel.c          # Fuel sensor thread
│   ├── temp.c          # Temperature sensor thread
│   ├── logger.c        # JSON logging utilities
│   ├── utils.c         # Timestamp + helper functions
│   └── ui.c            # ncurses dashboard
│
├── include/
│   └── data.h          # Shared structs, thresholds, mutex/condvars
│
├── logs/               # Runtime logs (ignored by git)
│
├── Makefile            # Build script
├── .gitignore          # Ignore binary, object files, logs
└── README.md           # Project documentation
```

---

## 🛠️ Build & Run

### **1️⃣ Build**
```bash
make
```

### **2️⃣ Run**
```bash
./vehicle_monitor
```

### **3️⃣ Stop (Gracefully)**
Press:

```
Ctrl + C
```

The program intercepts SIGINT, signals all threads to terminate, flushes logs, and exits cleanly.

---

## 📝 Sample JSON Log Entry

```json
{
  "timestamp": "2025-11-12 14:22:01",
  "sensor": "speed",
  "event": "SPEED_HIGH",
  "value": 102
}
```

Logs are stored in:

```
logs/vehicle.log
```

(Logs are excluded from Git to keep the repository clean.)

---

## 📌 Module Responsibilities

### **🧵 main.c**
- Initialize system state  
- Setup mutexes + condition variables  
- Launch sensor, logger, and UI threads  
- Manage SIGINT + graceful shutdown  

### **🚀 speed.c / fuel.c / temp.c**
- Generate simulated sensor readings  
- Apply hysteresis thresholds  
- Apply debounce logic  
- Update shared state  

### **🪵 logger.c**
- Thread-safe write operations  
- JSON formatting  
- Timestamping  

### **🔧 utils.c**
- Timestamp helpers  
- Small utility wrappers  

### **📺 ui.c**
- Ncurses dashboard  
- Display sensor values + alerts  
- Refresh loop with clean exit  

---

## 📈 Future Enhancements

- Configurable thresholds from a JSON file  
- Socket-based remote monitoring  
- Log rotation + size limits  
- Hardware sensor input (UART/SPI/CAN)  
- Thread CPU affinity for scheduling optimization  

---

## 👨‍💻 Author

**Harish S**  
Embedded Linux & System Programming Engineer  
📧 **harishnathan024@gmail.com**  
🔗 LinkedIn: (https://www.linkedin.com/in/harish-s-embedded/)

---

