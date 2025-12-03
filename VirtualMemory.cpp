#include "VirtualMemory.h"
#include <iostream>
#include <algorithm>

VirtualMemory::VirtualMemory(int tlbSize, int /* pageCount */) 
    : tlbSize(tlbSize), currentTime(0) {
    tlb.resize(tlbSize);
    for (int i = 0; i < tlbSize; i++) {
        tlb[i].pid = -1;
        tlb[i].pageNumber = -1;
        tlb[i].frameNumber = -1;
        tlb[i].lastUsed = -1;
    }
}

bool VirtualMemory::lookupTLB(int pid, int virtualAddress, int& physicalAddress) {
    // TODO: Implement TLB lookup
    // Extract page number from virtual address (shift right by 12 bits for 4KB pages)
    int pageNumber = virtualAddress >> 12;
    
    // Check TLB for hit
    for (int i = 0; i < tlbSize; i++) {
        if (tlb[i].pid == pid && tlb[i].pageNumber == pageNumber) {
            // TLB hit
            tlb[i].lastUsed = currentTime++;
            int offset = virtualAddress & 0xFFF; // Last 12 bits
            physicalAddress = (tlb[i].frameNumber << 12) | offset;
            return true;
        }
    }
    
    // TLB miss
    return false;
}

bool VirtualMemory::translateAddress(int pid, int virtualAddress, int& physicalAddress) {
    // TODO: Implement two-level page table translation
    // Split 32-bit virtual address: 10 bits outer page, 10 bits inner page, 12-bit offset
    int outerPage = (virtualAddress >> 22) & 0x3FF; // Bits 22-31 (10 bits)
    int innerPage = (virtualAddress >> 12) & 0x3FF; // Bits 12-21 (10 bits)
    int offset = virtualAddress & 0xFFF;             // Bits 0-11 (12 bits)
    
    // Look up outer page table
    auto outerKey = std::make_pair(pid, outerPage);
    if (outerPageTable.find(outerKey) == outerPageTable.end()) {
        return false; // Page fault
    }
    
    // Look up inner page table
    auto innerKey = std::make_pair(pid, innerPage);
    if (innerPageTable.find(innerKey) == innerPageTable.end()) {
        return false; // Page fault
    }
    
    // Calculate physical address
    int frameNumber = innerPageTable[innerKey];
    physicalAddress = (frameNumber << 12) | offset;
    
    return true;
}

void VirtualMemory::insertTLB(int pid, int pageNumber, int frameNumber) {
    // TODO: Implement TLB insertion with LRU eviction
    int lruIndex = 0;
    int minTime = tlb[0].lastUsed;
    
    // Find LRU entry or empty slot
    for (int i = 0; i < tlbSize; i++) {
        if (tlb[i].pid == -1) {
            // Found empty slot
            lruIndex = i;
            break;
        }
        if (tlb[i].lastUsed < minTime) {
            minTime = tlb[i].lastUsed;
            lruIndex = i;
        }
    }
    
    // Insert new entry
    tlb[lruIndex].pid = pid;
    tlb[lruIndex].pageNumber = pageNumber;
    tlb[lruIndex].frameNumber = frameNumber;
    tlb[lruIndex].lastUsed = currentTime++;
}

void VirtualMemory::loadPage(int pid, int pageNumber, int frameNumber) {
    // TODO: Update the two-level page table
    int outerPage = (pageNumber >> 10) & 0x3FF;
    int innerPage = pageNumber & 0x3FF;
    
    auto outerKey = std::make_pair(pid, outerPage);
    auto innerKey = std::make_pair(pid, innerPage);
    
    // Mark outer page table entry as valid
    outerPageTable[outerKey] = 1;
    
    // Insert mapping in inner page table
    innerPageTable[innerKey] = frameNumber;
}

void VirtualMemory::printStats() const {
    std::cout << "\n=== Virtual Memory Statistics ===" << std::endl;
    std::cout << "TLB Entries: " << tlbSize << std::endl;
    std::cout << "Current Time: " << currentTime << std::endl;
}
