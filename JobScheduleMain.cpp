#include "JonSchedule.h"

using namespace std;

int main(){
    vector<JobControlBlock> jobs = {
        JobControlBlock(1, Ready, 1, 5, 0),
        JobControlBlock(2, Ready, 2, 3, 1),
        JobControlBlock(3, Ready, 3, 8, 2),
        JobControlBlock(4, Ready, 4, 6, 3),
        JobControlBlock(5, Ready, 5, 2, 4)
    };
    JobSchduleBase* FCFSschdule = new FCFSSchdule(jobs);
}