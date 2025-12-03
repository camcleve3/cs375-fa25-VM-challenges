# Virtual Memory Simulator — Minimal Analysis Report

**Name:** [Your Name]
**Course:** CS375 — Operating Systems
**Assignment:** Virtual Memory Concepts and Challenges
**Date:** December 2, 2025

## Configuration
- TLB: 8 entries
- Physical memory: 16 frames
- Page size: 4KB
- Address split: 10 (outer) / 10 (inner) / 12 (offset)
- Replacement: Clock (Second-Chance)

## Input
File: `input/trace.txt`
```
1 0x00001000
1 0x00002004
2 0x00010000
3 0x0000F000
1 0x00001008
```

## Results (from program output)
- Total accesses: 5
- TLB hits: 1 (20%)
- TLB misses: 4 (80%)
- Page faults: 4 (80%)

## Observations
- Cold-start accesses cause TLB misses and page faults; repeated access to the same page yields a TLB hit.
- With 16 frames and only 4 pages touched, Clock did not need to evict; reference bits would drive victim selection once >16 unique pages are accessed.

## Clock Algorithm Note
- Victim selection: scan frames; if referenced=1, clear and advance; if referenced=0, evict.
- No evictions occurred for this trace; verify behavior by accessing >16 distinct pages.

## Conclusion
- TLB improves translation speed after initial misses; hit rate rises with locality.
- Page faults dominate cost on first touches; keeping the working set within 16 frames avoids evictions.
- Configuration meets assignment requirements; results align with expected demand paging behavior.

## Appendix (paste screenshot or short log)
Example excerpt:
```
Access #5: PID=1 Virtual=0x1008 -> TLB HIT -> Physical=0x8

=== Simulation Statistics ===
Total Memory Accesses: 5
TLB Hits: 1 (20%)
Page Faults: 4 (80%)
```
