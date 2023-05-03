#include "ProcessScheduling.h"

using namespace std;

int main() {
    vector<ProcessControlBlock> processes = {
        ProcessControlBlock(1, Ready, 1, 5, 4),
        ProcessControlBlock(2, Ready, 2, 3, 1),
        ProcessControlBlock(3, Ready, 3, 8, 1),
        ProcessControlBlock(4, Ready, 4, 6, 3),
        ProcessControlBlock(5, Ready, 5, 2, 4)
    };
    ProcessScheduling PD(processes);
    PD.startScheduling();
    PD.outputRes();
    return 0;
}