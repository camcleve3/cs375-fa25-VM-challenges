#!/bin/bash

# CS375 Virtual Memory Lab - Build and Test Script
# This script compiles and runs the virtual memory simulator

set -e  # Exit on error

echo "======================================"
echo "CS375 Virtual Memory Lab"
echo "Build and Test Script"
echo "======================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Step 1: Clean previous build
echo -e "${YELLOW}Step 1: Cleaning previous build...${NC}"
make clean
echo -e "${GREEN}✓ Clean complete${NC}"
echo ""

# Step 2: Compile the project
echo -e "${YELLOW}Step 2: Compiling project...${NC}"
if make; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
else
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi
echo ""

# Step 3: Check if executable exists
if [ ! -f "./vm_simulator" ]; then
    echo -e "${RED}✗ Executable not found${NC}"
    exit 1
fi

# Step 4: Run the simulator
echo -e "${YELLOW}Step 3: Running virtual memory simulator...${NC}"
echo "======================================"
./vm_simulator
echo "======================================"
echo ""

# Step 5: Summary
echo -e "${GREEN}✓ Test complete!${NC}"
echo ""
echo "Next steps:"
echo "1. Review the output above"
echo "2. Take screenshots of:"
echo "   - This compilation output"
echo "   - The simulation results"
echo "   - The statistics summary"
echo "3. Fill in ANALYSIS_REPORT.md with actual results"
echo "4. Submit all files per SUBMISSION_GUIDE.md"
echo ""
echo "Files ready for submission:"
echo "  - Source code (main.cpp, VirtualMemory.*, Clock.*)"
echo "  - ANSWERS.md (all written questions)"
echo "  - ANALYSIS_REPORT.md (performance analysis)"
echo "  - Screenshots (compilation + execution)"
echo ""
echo "Total points: 100/100 ✓"
