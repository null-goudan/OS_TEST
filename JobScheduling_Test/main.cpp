#include "JobScheduling.h"

using namespace std;

static bool SELFCompare(JobControlBlock& a, JobControlBlock& b){
    if (a.arrival_time == b.arrival_time) {
        return a.id < b.id;
    }
    return a.arrival_time < b.arrival_time;
}

int main() {
    vector<JobControlBlock> jobs = {
        JobControlBlock(1, Ready, 1, 5, 0),
        JobControlBlock(2, Ready, 2, 3, 1),
        JobControlBlock(3, Ready, 3, 8, 2),
        JobControlBlock(4, Ready, 4, 6, 3),
        JobControlBlock(5, Ready, 5, 2, 4)
    };
    JobScheduling* FCFSscheduling = new JobScheduling(jobs, FCFS_METHOD);
    JobScheduling* SJFscheduling = new JobScheduling(jobs, SJF_METHOD);
    JobScheduling* SELFscheduling = new JobScheduling(jobs, SELF_METHOD);

    
    FCFSscheduling->scheduling(); FCFSscheduling->outputRes();
    SJFscheduling->scheduling(); SJFscheduling->outputRes();
    SELFscheduling->ChangeSelfMethod(SELFCompare);
    SELFscheduling->scheduling(); SELFscheduling->outputRes();
    return 0;
}