#include <iostream>
#include <fstream>
#include <string>
#include "VirtualMemory.h"
#include "Clock.h"

int main() {
    // Configuration
    const int TLB_SIZE = 8;
    const int FRAME_COUNT = 16;
    const int PAGE_COUNT = 1024; // For demo purposes
    
    // Initialize virtual memory system and Clock algorithm
    VirtualMemory vm(TLB_SIZE, PAGE_COUNT);
    Clock clock(FRAME_COUNT);
    
    // Statistics
    int tlbHits = 0, tlbMisses = 0;
    int pageFaults = 0;
    int totalAccesses = 0;
    
    // Read input trace file
    std::ifstream traceFile("input/trace.txt");
    if (!traceFile.is_open()) {
        std::cerr << "Error: Could not open input/trace.txt" << std::endl;
        return 1;
    }
    
    std::string format;
    std::getline(traceFile, format); // Read format line
    
    int pid;
    unsigned int virtualAddress;
    
    std::cout << "=== Virtual Memory Simulator ===" << std::endl;
    std::cout << "TLB Size: " << TLB_SIZE << " entries" << std::endl;
    std::cout << "Physical Memory: " << FRAME_COUNT << " frames" << std::endl;
    std::cout << "Page Size: 4KB (4096 bytes)" << std::endl;
    std::cout << "\nProcessing memory accesses...\n" << std::endl;
    
    while (traceFile >> pid >> std::hex >> virtualAddress) {
        totalAccesses++;
        int physicalAddress;
        
        std::cout << "Access #" << totalAccesses << ": PID=" << pid 
                  << " Virtual=0x" << std::hex << virtualAddress << std::dec;
        
        // Step 1: Check TLB
        if (vm.lookupTLB(pid, virtualAddress, physicalAddress)) {
            tlbHits++;
            std::cout << " -> TLB HIT -> Physical=0x" << std::hex 
                      << physicalAddress << std::dec << std::endl;
        } else {
            tlbMisses++;
            std::cout << " -> TLB MISS";
            
            // Step 2: Check page table
            if (vm.translateAddress(pid, virtualAddress, physicalAddress)) {
                std::cout << " -> Page Table HIT -> Physical=0x" 
                          << std::hex << physicalAddress << std::dec << std::endl;
                
                // Update TLB
                int pageNumber = virtualAddress >> 12;
                int frameNumber = physicalAddress >> 12;
                vm.insertTLB(pid, pageNumber, frameNumber);
            } else {
                // Page fault
                pageFaults++;
                std::cout << " -> PAGE FAULT" << std::endl;
                
                int pageNumber = virtualAddress >> 12;
                int evictedPage;
                int frameNumber = clock.evictAndReplace(pid, pageNumber, evictedPage);
                
                // Load page into memory
                vm.loadPage(pid, pageNumber, frameNumber);
                
                // Update TLB
                vm.insertTLB(pid, pageNumber, frameNumber);
                
                // Calculate physical address
                int offset = virtualAddress & 0xFFF;
                physicalAddress = (frameNumber << 12) | offset;
                std::cout << "    Loaded page " << pageNumber 
                          << " into frame " << frameNumber 
                          << " -> Physical=0x" << std::hex << physicalAddress 
                          << std::dec << std::endl;
            }
        }
    }
    
    traceFile.close();
    
    // Print statistics
    std::cout << "\n=== Simulation Statistics ===" << std::endl;
    std::cout << "Total Memory Accesses: " << totalAccesses << std::endl;
    std::cout << "TLB Hits: " << tlbHits << " (" 
              << (100.0 * tlbHits / totalAccesses) << "%)" << std::endl;
    std::cout << "TLB Misses: " << tlbMisses << " (" 
              << (100.0 * tlbMisses / totalAccesses) << "%)" << std::endl;
    std::cout << "Page Faults: " << pageFaults << " (" 
              << (100.0 * pageFaults / totalAccesses) << "%)" << std::endl;
    std::cout << "TLB Hit Rate: " << (100.0 * tlbHits / totalAccesses) << "%" << std::endl;
    std::cout << "Page Fault Rate: " << (100.0 * pageFaults / totalAccesses) << "%" << std::endl;
    
    vm.printStats();
    
    return 0;
}
