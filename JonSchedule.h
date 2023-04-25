#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct JobControlBlock;
typedef vector<JobControlBlock> JobQueue;
class JobSchduleBase;
class FCFSSchdule;
class SJFSchdule;

enum ProcessStatus{
    Ready = 0,          // 就绪态
    Run = 1,            // 工作态
    Complete = 2,       // 完成
};

struct JobControlBlock
{
    JobControlBlock()=default;
    JobControlBlock(int id, ProcessStatus status, int arrival_time, int burst_time, int waiting_time = 0,
                    int completion_time = -1, int priority = 0)
    :id(id), status(status), arrival_time(arrival_time),burst_time(burst_time), waiting_time(waiting_time)
    ,completion_time(completion_time),priority(priority){}

    int             id;                         // 进程ID
    ProcessStatus   status;                     // 进程状态
    int             arrival_time;               // 到达时间
    int             burst_time;                 // 需要执行时间
    int             waiting_time;               // 等待时间
    int             completion_time;            // 完成时间
    int             priority;                   // 进程优先级
};

class JobSchduleBase{
    public:
        JobSchduleBase():job_queue(new vector<JobControlBlock>()){};
        JobSchduleBase(vector<JobControlBlock>& jobs):job_queue (new vector<JobControlBlock>(jobs)){};
        virtual ~JobSchduleBase()=0;
        virtual void schduling()=0;
        void outputRes();
    protected:
        JobQueue    *job_queue;
        string      _algorithm_name;
};

class FCFSSchdule: public JobSchduleBase{
    public:
        FCFSSchdule(){};
        FCFSSchdule(vector<JobControlBlock>& jobs):JobSchduleBase(jobs){};
        ~FCFSSchdule() override;
        void schduling() override;
    private:
        // 不需要继续继承下去的算法
        void FCFS();
};

class SJFSchdule: public JobSchduleBase{
    public:
        SJFSchdule(){};
        SJFSchdule(vector<JobControlBlock>& jobs):JobSchduleBase(jobs){};
        ~SJFSchdule() override;
        void schduling() override;
    private:
        void SJF();
};

