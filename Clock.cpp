#include "Clock.h"
#include <iostream>

Clock::Clock(int frameCount) : frameCount(frameCount), clockHand(0) {
    frames.resize(frameCount);
    for (int i = 0; i < frameCount; i++) {
        frames[i].pid = -1;
        frames[i].pageNumber = -1;
        frames[i].referenced = false;
    }
}

int Clock::evictAndReplace(int pid, int pageNumber, int& evictedPage) {
    // TODO: Implement Clock algorithm to select a victim frame
    // Use the Clock algorithm with reference bits
    
    while (true) {
        // Check if current frame is empty
        if (frames[clockHand].pid == -1) {
            // Found empty frame
            int victimFrame = clockHand;
            frames[victimFrame].pid = pid;
            frames[victimFrame].pageNumber = pageNumber;
            frames[victimFrame].referenced = true;
            clockHand = (clockHand + 1) % frameCount;
            evictedPage = -1; // No page was evicted
            return victimFrame;
        }
        
        // Check reference bit
        if (!frames[clockHand].referenced) {
            // Found victim - reference bit is 0
            int victimFrame = clockHand;
            evictedPage = frames[victimFrame].pageNumber;
            std::cout << "Evicting page " << evictedPage 
                      << " from frame " << victimFrame << std::endl;
            
            frames[victimFrame].pid = pid;
            frames[victimFrame].pageNumber = pageNumber;
            frames[victimFrame].referenced = true;
            clockHand = (clockHand + 1) % frameCount;
            return victimFrame;
        }
        
        // Reference bit is 1, set to 0 and move to next frame
        frames[clockHand].referenced = false;
        clockHand = (clockHand + 1) % frameCount;
    }
}
