#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
#define FCFS_METHOD 1
#define SJF_METHOD  2
#define SELF_METHOD 3

enum JobStatus{
    Ready = 0,          // 就绪态
    Run = 1,            // 工作态
    Complete = 2,       // 完成
};

struct JobControlBlock
{
    JobControlBlock()=default;
    JobControlBlock(int id, JobStatus status, int arrival_time, int burst_time, int waiting_time = 0,
                    int completion_time = -1, int priority = 0)
    :id(id), status(status), arrival_time(arrival_time),burst_time(burst_time), waiting_time(waiting_time)
    ,completion_time(completion_time),priority(priority){}

    int             id;                         // 作业ID
    JobStatus       status;                     // 作业状态
    int             arrival_time;               // 到达时间
    int             burst_time;                 // 需要执行时间
    int             waiting_time;               // 等待时间
    int             completion_time;            // 完成时间
    int             priority;                   // 作业优先级
};

class Job



