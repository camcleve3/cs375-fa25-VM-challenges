#ifndef CLOCK_H
#define CLOCK_H

#include <vector>
#include <cstddef>

// Frame Entry for Clock algorithm
struct FrameEntry {
    int pid;
    int pageNumber;
    bool referenced;
};

// Clock page replacement algorithm
class Clock {
private:
    std::vector<FrameEntry> frames;
    int frameCount;
    size_t clockHand;

public:
    Clock(int frameCount);
    int evictAndReplace(int pid, int pageNumber, int& evictedPage);
};

#endif // CLOCK_H
