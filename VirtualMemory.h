#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <vector>
#include <map>
#include <utility>

// TLB Entry structure
class TLBEntry {
public:
    int pid;
    int pageNumber;
    int frameNumber;
    int lastUsed;
};

// Virtual Memory class with TLB and two-level page table
class VirtualMemory {
private:
    std::vector<TLBEntry> tlb; // TLB with fixed size
    std::map<std::pair<int, int>, int> outerPageTable; // (pid, outer_page) -> inner table
    std::map<std::pair<int, int>, int> innerPageTable; // (pid, inner_page) -> frame#
    int tlbSize;
    int currentTime;

public:
    VirtualMemory(int tlbSize, int pageCount);
    bool lookupTLB(int pid, int virtualAddress, int& physicalAddress);
    bool translateAddress(int pid, int virtualAddress, int& physicalAddress);
    void insertTLB(int pid, int pageNumber, int frameNumber);
    void loadPage(int pid, int pageNumber, int frameNumber);
    void printStats() const;
};

#endif // VIRTUALMEMORY_H
