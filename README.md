# 🖥️ FOS — Faculty Operating System
![C](https://img.shields.io/badge/C-99%25-A8B9CC?style=flat-square&logo=c&logoColor=white)
![x86](https://img.shields.io/badge/x86-32--bit-red?style=flat-square&logo=intel&logoColor=white)
![Bochs](https://img.shields.io/badge/Bochs-Emulator-blue?style=flat-square)
![Status](https://img.shields.io/badge/Status-Complete-10b981?style=flat-square)
![Course](https://img.shields.io/badge/Course-Operating_Systems-purple?style=flat-square)
![Tests](https://img.shields.io/badge/Tests-10%2B-brightgreen?style=flat-square)

**A fully-functional x86 kernel implementing virtual memory management, dynamic allocation, and CPU scheduling**

> Educational OS from Ain Shams University. ~2,000 lines of C kernel code + comprehensive testing suite. Demonstrates systems programming mastery: paging, memory management, fault handling, and scheduler design.

---

## ⚡ Key Features at a Glance

| What | How | Why It Matters |
|------|-----|---|
| **Virtual Memory** | Page-based allocation + working sets | Enables processes larger than physical RAM |
| **Page Replacement** | FIFO, LRU, CLOCK, Modified CLOCK | Trade-off between simplicity and optimality |
| **Kernel Heap** | First Fit strategy | Tests memory fragmentation handling |
| **User Heap** | malloc/free with Best Fit strategy | Real-world heap behavior simulation |
| **Page Faults** | On-demand loading + placement | Handles memory exceptions at CPU level |
| **CPU Scheduling** | Round Robin + MLFQ | Compares fairness vs throughput |

---

## 🎯 What This Demonstrates

✅ **Systems Programming:** Low-level C code managing CPU interrupts, page tables, and memory hierarchies  
✅ **OS Fundamentals:** Virtual memory, paging, context switching, interrupt handling  
✅ **Algorithm Analysis:** Implemented 4 replacement algorithms; measured tradeoffs  
✅ **Embedded Thinking:** Ran on Bochs x86 emulator with real hardware constraints  
✅ **Testing Discipline:** Built 10+ test programs to validate each subsystem  

---

## 🚀 Quick Start

### Prerequisites
- GCC with 32-bit support (or i386-elf cross-compiler)
- GNU Make
- Bochs x86 emulator

### Build
```bash
cd /path/to/project
make clean
make
```

### Run
```bash
bochs -f .bochsrc

# Or on Windows:
FOS_Developer_Console.bat
```

You'll see the FOS kernel prompt:
```
FOS> 
```

### First Commands to Try
```bash
FOS> help                         # Show all commands
FOS> run tm1 2000                # Test malloc
FOS> sched?                      # Show current scheduler
FOS> run fos_helloWorld 5        # Run hello world program
```

---

## 📊 Architecture Overview

```
User Programs (Test Suite)
          ↓
    System Calls
          ↓
┌─────────────────────────────────┐
│      FOS Kernel (2K+ LoC)       │
├─────────────────────────────────┤
│ ► Memory Manager                │
│   - Frame allocation            │
│   - Page table management       │
│   - Working set tracking        │
│                                 │
│ ► Page Fault Handler (trap.c)   │
│   - On-demand page loading      │
│   - Placement & replacement     │
│                                 │
│ ► Kernel Heap (kheap.c)         │
│   - Dynamic kernel allocation   │
│   - Fragmentation management    │
│                                 │
│ ► CPU Scheduler                 │
│   - Round Robin                 │
│   - Multi-Level Feedback Queue  │
│                                 │
│ ► Command Interpreter           │
│   - 20+ interactive commands    │
└─────────────────────────────────┘
          ↓
    Physical Memory (Simulated)
```

---

## 💻 Available Commands

### Process Management
```bash
FOS> run <program> <working_set_size>    # Load and execute
FOS> load <program> <working_set_size>   # Load, don't run yet
FOS> runall                               # Run all loaded programs
FOS> kill <env_id>                        # Terminate process
```

### Memory Strategy Configuration
```bash
FOS> uhbestfit      # User heap: Best Fit (default)
FOS> khbestfit      # Kernel heap: Best Fit
FOS> uheap?         # Show current user heap strategy
```

### Page Replacement Algorithm Selection
```bash
FOS> lru            # Least Recently Used
FOS> clock          # CLOCK algorithm
FOS> modifiedclock  # CLOCK with write tracking
FOS> rep?           # Show current algorithm
```

### Scheduler Configuration
```bash
FOS> schedRR 100                    # Round Robin, 100ms quantum
FOS> schedMLFQ 3 100 200 400        # MLFQ: 3 levels, 100/200/400ms quanta
```

---

## ✅ Testing & Validation

### Kernel Heap Tests
```bash
FOS> khfirstfit        # Set kernel heap strategy
FOS> tstkmalloc 1      # Test kmalloc basic allocation
FOS> tstkmalloc 2      # Test kmalloc with kfree
FOS> tstkfree          # Test kfree
FOS> tstkvirtaddr     # Test virtual address translation
FOS> tstkphysaddr     # Test physical address translation
```

### Page Fault Handler Tests
```bash
FOS> run tpp 20        # Test page placement
FOS> run tia 15        # Test invalid memory access handling
FOS> run tpr1 11       # Test page replacement (allocation)
FOS> run tpr2 6        # Test page replacement (stack)
FOS> run tmodclk 11    # Test Modified CLOCK algorithm
FOS> run tpb1 11       # Test page buffering
FOS> modbufflength 10
FOS> run tpb2 11       # Test modified frame list
FOS> run tpb3 11       # Test buffered page removal
```

### User Heap Tests
```bash
FOS> run tm1 2000      # Test malloc #1 (addresses & frames)
FOS> run tm2 2000      # Test malloc #2 (read/write access)
FOS> run tm3 2000      # Test malloc #3 (WS after access)
FOS> run tf1 2000      # Test free #1 (page file & WS cleanup)
FOS> run tf2 2000      # Test free #2 (access after free)
```

### Environment Tests
```bash
FOS> run tef1 10       # Test env_free without malloc
FOS> run tef2 20       # Test env_free with malloc
```

**Why this matters:** Each test validates a critical OS subsystem in isolation.

---

## 🏗️ Core Components

| Component | File | Purpose |
|-----------|------|---------|
| **Memory Manager** | `kern/memory_manager.c` | Page allocation, frame tracking, working sets |
| **Page Fault Handler** | `kern/trap.c` | Handles CPU page fault interrupts, loads pages on demand |
| **Kernel Heap** | `kern/kheap.c` | Dynamic memory for kernel (kmalloc/kfree) |
| **User Heap** | `lib/uheap.c` | malloc/free for user programs |
| **Scheduler** | `kern/sched.c` | Round Robin + MLFQ CPU scheduling |
| **System Calls** | `kern/syscall.c` | User/kernel boundary interface |
| **Command Interpreter** | `kern/command_prompt.c` | Interactive kernel shell |

---

## 🗺️ Memory Layout

```
User/Kernel Virtual Address Space (32-bit x86)

0xFFFFFFFF ┌─────────────────────────┐
           │  Kernel Code & Data     │  Read-only, supervisor-only
0xF0000000 ├─────────────────────────┤  KERNEL_BASE
           │  Kernel Heap (dynamic)  │  kmalloc region
0xE0000000 ├─────────────────────────┤  KERNEL_HEAP_START
           │        (gap)            │
0x80B00000 ├─────────────────────────┤
           │  User Heap (dynamic)    │  malloc region
0x80000000 ├─────────────────────────┤  USER_HEAP_START
           │  User Stack (grows ↓)   │
0x7FFFFFFF └─────────────────────────┘
```

**Key design:** Kernel occupies top 256MB of address space. User has 2GB. Heap and stack separate regions.

---

## 🔍 Interesting Design Decisions

### 1. **Why Four Heap Placement Strategies?**
- **First Fit:** Fast, simple, but fragmentation
- **Best Fit:** Minimizes fragmentation, slower
- **Next Fit:** Spread fragmentation, moderate speed
- **Worst Fit:** Balanced but rarely used in practice

Testing all four teaches the **algorithm vs. implementation tradeoff.**

### 2. **Why Multiple Page Replacement Algorithms?**
- **FIFO:** Simplest, but doesn't account for usage
- **LRU:** Optimal theoretical choice, expensive to implement
- **CLOCK:** LRU approximation with 1 reference bit (practical)
- **Modified CLOCK:** Adds write tracking to minimize disk I/O

Shows **how real OS kernels balance theory with efficiency.**

### 3. **Why MLFQ Scheduler?**
Round Robin alone treats all processes equally. MLFQ adds **priority feedback:**
- Interactive jobs (short bursts) stay in high-priority queue
- Batch jobs (long CPU) demote to low-priority queue
- Kernel can adapt scheduling to workload dynamically

---

## 📈 What Recruiters See

✅ **You understand the full OS stack** — from CPU interrupts to user malloc  
✅ **You can implement complex algorithms** — paging, scheduling, memory management  
✅ **You tested rigorously** — 10+ test programs, not just "it compiles"  
✅ **You handled real constraints** — x86 emulation, page faults, context switches  
✅ **You documented your thinking** — design decisions explained above  

---

## 🏫 Academic Context

**Faculty of Computer and Information Sciences**  
**Ain Shams University, Cairo, Egypt**  
**Operating Systems Course (2024–2025)**

This was a semester-long capstone project demonstrating mastery of OS fundamentals before moving to kernel development internships or graduate study.

---

## 📝 How to Build/Run

### Prerequisites
- GCC (Linux/WSL)
- Bochs x86 emulator
- GNU Make

### Build
```bash
make clean
make
```

### Run
```bash
# Linux/WSL
bochs -f .bochsrc

# Windows (if .bat provided)
FOS_Developer_Console.bat
```

### First Commands to Try
```bash
FOS> run tm1 2000       # Quick malloc test
FOS> sched?             # See current scheduler
FOS> schedMLFQ 2 100 200  # Try MLFQ
FOS> help               # List all commands
```

---

## 🚀 If You Were Hiring

**What makes this impressive:**
- **Real code:** Not a toy scheduler or fake paging algorithm
- **Tested:** Multiple test programs validate each component
- **Constrained:** Had to work within x86 architecture limits
- **Thoughtful:** Design decisions documented
- **Extensible:** Easy to swap algorithms (FIFO ↔ LRU ↔ CLOCK)

---

*Built from scratch. Bootloader to shell. ~2,000 lines of kernel C.*
"# x86-kernel-implementation" 
#   x 8 6 - k e r n e l - i m p l e m e n t a t i o n  
 