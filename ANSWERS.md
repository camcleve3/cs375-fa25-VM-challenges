## CS375 – Virtual Memory Concepts & Simulator Analysis
# Cameron Cleveland
# Final Written Report + Simulation Output
## 1. Simulation Screenshots

## Compilation (make clean + make)
![Compilation succeeded](compilation.png)

## Virtual Memory Simulator Execution
![VM simulator run](run-output.png)

## 2. One-Page Analysis Report
# Virtual Memory System Behavior Analysis

This simulation demonstrates how demand paging, TLB caching, and the Clock page-replacement algorithm work together during virtual-to-physical memory translation. With a TLB size of 8 entries and 16 physical frames, the system shows typical cold-start behavior, where early accesses generate many misses.

**TLB Performance**
**TLB Hits:** 1
**TLB Misses:** 4
**TLB Hit Rate:** 20%

Because the first several accesses reference different pages from different processes (PID 1 → PID 1 → PID 2 → PID 3), the TLB had no opportunity to reuse cached translations. Only access #5 reused PID 1’s page, producing the single hit.
Low hit rates are normal at startup. As the working set stabilizes, the TLB hit rate typically increases substantially.

**Key Point:**
A small TLB + multiple processes = many initial misses. Once reused pages reappear, hit rates improve.

**Page Fault Behavior**
**Page Faults:** 4
**Page Fault Rate:** 80%

Every new page reference produced a fault because none of the pages had been loaded yet. This reflects the expected cost of demand paging during early execution, not poor replacement behavior.
Since only four unique pages were accessed, and physical memory holds 16 frames, no evictions occurred.

**Clock Algorithm Evaluation**
Although the Clock algorithm did not perform a full eviction during this trace, its behavior is predictable:
- Recently accessed pages get a “second chance.”
- Pages touched once (like PID 3’s page) would be evicted before heavily reused pages.
- It approximates LRU with far lower overhead.
In larger workloads, Clock would outperform FIFO and approach LRU performance without expensive tracking structures.

**Overall System Assessment**
The simulator accurately reflects core virtual memory principles:
- High early TLB misses and high page faults are normal during cold start.
- Demand paging loads pages on first access.
- Clock replacement ensures recency-aware eviction once memory fills.
- TLB caching provides substantial benefits after initial warm-up.
This trace is small, but the results demonstrate proper implementation and realistic system behavior.

## Part 2: Questions and Coding Tasks

---

## Short-Answer Questions (30 points)

### 1. Demand Paging (5 points)
**Question:** How does demand paging optimize memory usage compared to loading all pages at process startup? What is the trade-off?

**Answer:** Demand paging improves memory efficiency by only loading pages when a process actually needs them, instead of loading the entire program upfront. This lets the system support more processes at once because unused pages don’t take up RAM. The trade-off is that the first access to an unloaded page triggers a page fault, which introduces significant delay due to disk I/O.

---

### 2. Page Replacement Algorithms (5 points)
**Question:** Why might the Clock algorithm be preferred over FIFO in systems with limited memory? Provide a scenario where FIFO could perform better.

**Answer:** The Clock algorithm is often preferred over FIFO because it makes a smarter decision about which pages are “cold” based on their reference bits. This prevents frequently used pages from being evicted unnecessarily. FIFO can perform better in rare situations like one-time, strictly sequential access patterns where pages are never reused—so tracking “recent use” doesn’t provide any advantage.

---

### 3. TLB Functionality (5 points)
**Question:** Explain how a TLB miss is resolved and why it's slower than a TLB hit. What happens if the page is not in memory?

**Answer:** A TLB miss forces the system to fall back to a page table walk in main memory, which is much slower than a 1–2 cycle TLB hit. If the page table walk shows that the page isn’t in memory, a page fault occurs and the OS has to fetch the page from disk. That disk access is extremely slow compared to normal memory operations.

---

### 4. Address Translation (5 points)
**Question:** Describe how a virtual address is translated using a single-level page table. What role does the offset play?

**Answer:** With a single-level page table, the virtual address is divided into a page number and an offset. The page number indexes into the page table to obtain the physical frame number. The offset doesn’t change—it simply gets appended to the frame number to compute the final physical address.

---

### 5. Multi-Level Page Tables (5 points)
**Question:** How does a two-level page table reduce memory overhead compared to a single-level page table? Give an example.

**Answer:** A two-level page table saves memory because it only allocates inner tables for virtual address regions the process actually uses. For example, in a 32-bit system with 4KB pages, a single-level page table would require a full 4MB structure per process. A two-level table allocates small 4KB inner tables on demand, which can reduce memory use by orders of magnitude for sparse address spaces.

---

### 6. Caching Trade-offs (5 points)
**Question:** Why might increasing TLB size improve performance? What are the potential downsides?

**Answer:** A larger TLB can improve performance because a bigger TLB reduces misses and avoids costly page table walks. However, larger TLBs are more expensive to build, consume more power, and may increase lookup latency if the structure becomes too large or too associative. At some point, the added size no longer provides meaningful performance benefits.

---

## Long-Answer Questions (40 points)

### 1. Page Fault Handling (10 points)
**Question:** Describe the complete process of handling a page fault in a demand paging system, including the roles of the TLB, page table, and disk. How can frequent page faults degrade performance, and what is one strategy to reduce them?

**Answer:** 

In a demand paging system, a page fault happens when a process tries to access a virtual page that currently isn’t in memory. First, the CPU checks the TLB. If the mapping isn’t there, it performs a page table walk. If the page table entry shows the page is not present, the MMU triggers a page fault exception and the OS takes over.
The OS identifies where the page is stored on disk and picks a physical frame to load it into. If no free frames are available, the OS uses a replacement algorithm—such as Clock or LRU—to select a victim page. Modified pages must be written back to disk before eviction. The OS then reads the requested page from disk into memory, updates the page table, and adds the new translation to the TLB.
Frequent page faults severely degrade performance because disk I/O is slow compared to RAM. Systems may even thrash if the working set doesn’t fit into memory. A common strategy to reduce page faults is allocating more frames to a process or using working set–based allocation so the process keeps the pages it actually uses.

---

### 2. Page Replacement Comparison (10 points)
**Question:** Compare the implementation complexity and performance of LRU versus Clock page replacement algorithms. Under what memory access patterns (e.g., sequential vs. random) might one outperform the other? Include a specific example.

**Answer:**

LRU tracks exact recency information, making it conceptually the “best” choice, but it’s expensive to implement. It requires updating metadata on every memory access, which adds overhead. The Clock algorithm gives a practical approximation of LRU using simple reference bits and a circular pointer, so it’s much easier to implement efficiently.
LRU typically performs best when programs show strong temporal locality—meaning recently used pages are likely to be used again. Clock performs similarly in these situations but with far less overhead. In sequential access patterns where pages are only used once, both algorithms perform similarly because neither recency nor reference bits help.
A clear example is a loop over pages 1–4 with only three frames available. Both LRU and Clock will identify that pages 1–3 are frequently used and will repeatedly evict page 4. FIFO, by comparison, would push out page 1 even though it’s needed again. This is why Clock is widely used—it gives LRU-like results while staying lightweight.

---

### 3. TLB and Caching (10 points)
**Question:** Explain how the TLB acts as a cache to improve address translation performance. Discuss the impact of TLB size on hit/miss rates and system performance. Why might a larger TLB not always be beneficial?

**Answer:**

The TLB serves as a fast cache for virtual-to-physical page translations, allowing the CPU to bypass slower multi-level page table walks. A TLB hit returns the frame number instantly, while a miss requires multiple memory accesses to walk the page table—significantly increasing latency.
Increasing TLB size usually improves the hit rate, especially for workloads with large working sets. Higher hit rates translate directly into faster memory access times and fewer page table walks. However, making the TLB too large can slow the lookup itself because associative hardware becomes more complex as entries increase. It also consumes more chip area and power, which is a serious trade-off.
This is why modern CPUs use multi-level TLBs: a small, fast L1 TLB backed by a larger L2 TLB provides strong coverage without slowing down the critical path for memory access. Bigger isn’t always better—the goal is balancing size, speed, and hardware cost.

---

### 4. Two-Level Page Tables (10 points)
**Question:** Illustrate how a two-level page table translates a virtual address into a physical address. Use a diagram or example with a 32-bit address and 4KB pages. Explain how this approach saves memory compared to a single-level page table.

**Answer:**

**Address Structure and Translation Process:**

Consider a 32-bit virtual address space with 4KB (2^12 byte) pages. The virtual address is divided as follows:

```
32-bit Virtual Address Layout:
|  10 bits  |  10 bits  |   12 bits    |
|  Outer    |  Inner    |    Offset    |
| Page Index| Page Index| (within page)|
```

- **Bits 31-22 (10 bits):** Outer page table index (2^10 = 1024 entries)
- **Bits 21-12 (10 bits):** Inner page table index (2^10 = 1024 entries)
- **Bits 11-0 (12 bits):** Offset within the page (4KB = 4096 bytes)

**Translation Steps - Example:**

Let's translate virtual address `0x00401234` (binary: `00000000010000000001001000110100`):

1. **Extract Indices:**
   - Outer index: bits 31-22 = `0000000001` = 1
   - Inner index: bits 21-12 = `0000000001` = 1
   - Offset: bits 11-0 = `001000110100` = 0x234 (564 bytes)

2. **Outer Page Table Lookup:**
   - Access the outer page table (pointed to by the page table base register)
   - Use outer index 1 to retrieve entry at `outerPageTable[1]`
   - This entry contains the physical address of inner page table #1

3. **Inner Page Table Lookup:**
   - Access inner page table #1 using its physical address
   - Use inner index 1 to retrieve entry at `innerPageTable[1]`
   - This entry contains the physical frame number, say frame 87 (0x57)

4. **Construct Physical Address:**
   - Combine frame number with offset: `(87 << 12) | 0x234 = 0x00057234`
   - Physical address: `0x00057234`

**Diagram:**
```
Virtual Address: 0x00401234
         ↓
┌────────┬────────┬──────────┐
│ Outer:1│ Inner:1│Offset:564│
└───┬────┴────┬───┴──────────┘
    │         │
    ↓         │
┌──────────┐  │
│ Outer PT │  │
│  [0]     │  │
│  [1]  ───┼──┘ points to Inner PT #1
│  [2]     │
└──────────┘
            ↓
        ┌──────────┐
        │Inner PT 1│
        │  [0]     │
        │  [1]: 87 │ ← Frame number
        │  [2]     │
        └──────────┘
                   ↓
        Physical: Frame 87, Offset 564
        = 0x00057234
```

**Memory Savings Compared to Single-Level:**

**Single-Level Page Table:**
- 32-bit address space with 4KB pages requires 2^20 page table entries
- Each entry typically 4 bytes (32-bit systems) or 8 bytes (64-bit)
- Total size per process: 2^20 × 4 bytes = 4MB (must be contiguous and always allocated)
- For 100 processes: 400MB of page table memory

**Two-Level Page Table:**
- Outer table: 1024 entries × 4 bytes = 4KB (always present)
- Inner tables: 1024 entries × 4 bytes = 4KB each (only allocated when needed)
- If a process uses only 4MB of address space (sparse distribution):
  - Needs only ~4 inner tables (assuming worst-case distribution)
  - Total: 4KB (outer) + 4×4KB (inner) = 20KB per process
- **Savings: 4MB - 20KB ≈ 99.5% reduction!**

**Key Advantage:** Inner tables are only allocated for address ranges actually used by the process. Most programs have sparse address spaces (code segment, heap, stack, with large unused gaps), so two-level tables waste far less memory. The outer table serves as a directory indicating which regions are valid, and only those regions have allocated inner tables.

**Real-World Impact:** This approach enables modern systems to support large address spaces efficiently. A 32-bit system can theoretically address 4GB per process, but most processes use far less. Two-level paging makes this practical by allocating page table memory proportional to actual usage rather than potential usage.


