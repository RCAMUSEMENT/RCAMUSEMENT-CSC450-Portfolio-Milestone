# CSC450: Programming III — Portfolio Milestone

**Student Name:** Ryley Carlson
**Course:** CSC450 - Programming III (Module 7)
**Development Environment:** Visual Studio Code (C/C++ Extension Pack)
**Compiler Toolchain:** GCC / G++ Cross-Platform Environment

---

## Project Overview
This repository contains the complete deliverables for the Module 7 Portfolio Milestone assignment. The project focuses on real-time multi-threaded scheduling, race-condition elimination, and cache-line aligned telemetry tracking:

1. **STRATEGIC SUBMERSIBLE TELEMETRY COMMAND (`main.cpp`):** A concurrent console application stylized as a deep-sea diving monitoring dashboard. The application is engineered to spawn two independent worker threads acting as coordinated counters to handle ballast tank flooding and hydrostatic pressure venting safely.
2. **System Design Pseudocode:** A high-level algorithmic blueprint detailing robust thread initialization check-points, explicit mutual exclusion lock scopes, and condition variable predicate validations to prevent data race conditions and state corruption.

---

## Hardware Optimization Note
While Eclipse IDE was originally recommended for this course, it utilizes a heavy GUI runtime framework that creates an overwhelming processing overhead on integrated graphics processing units. To mitigate UI thread blocking, asset delay bottlenecks, and local development system lag, this entire project environment was migrated to a highly optimized Visual Studio Code workspace configuration. This lean environment ensures that delayed cosmetic scrolling animations run smoothly without stalling background data-handling buffers.

---

## Compilation and Execution Instructions
To compile and run the source file manually via your local native terminal framework, execute the following compiler commands:

### 1. Submersible Telemetry Binary Compilation
```bash
g++ -std=c++11 -Wall main.cpp -o SubmersibleTelemetry
./SubmersibleTelemetry
```

---

## Identified Bug Fixes & Security Enhancements

### main.cpp Features:
* **Eliminated Core Context Switching Race Conditions:** Patched standard cross-thread timing hazards by wrapping shared variables inside strict `std::lock_guard<std::mutex>` scopes, completely preventing threads from executing raw split-write operations on the counter.
* **Mitigated CPU Spurious Wake-up Latencies:** Enforced an explicit lambda predicate loop (`[&telemetry] { return telemetry.divingPhaseComplete; }`) inside the condition variable wait sequence, locking the thread until variables are genuinely true and preventing core cycle drains.
* **Preserved Core-Coherence Cache Line Stability:** Engineered a strict `alignas(64)` hardware structural constraint on the telemetry data layout, forcing the shared variable fields onto their own independent cache line to eliminate false sharing delays across CPU cores.
* **Isolated Plaintext Data Memory Remanence Holes:** Stripped out dynamic heap-allocated `std::string` classes inside processing loops, passing raw string literals straight to a dedicated `consoleIOMutex` stream to insulate data logs from heap-scraping exposures.