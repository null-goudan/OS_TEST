#include "JobScheduling.h"
#include <thread>

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

    // 初始化好后，可以跑多线程模拟瞬时加入新作业的情况(给自己种个坑吧,有空再实现吧)
    // 如： thread1.join(scheduling) thread2.join(JoinNewJob) 
    FCFSscheduling->scheduling(); FCFSscheduling->outputRes();
    SJFscheduling->scheduling(); SJFscheduling->outputRes();
    SELFscheduling->ChangeSelfMethod(SELFCompare);
    SELFscheduling->scheduling(); SELFscheduling->outputRes();
    return 0;
}