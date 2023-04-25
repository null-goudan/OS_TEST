#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define FCFS_METHOD 1
#define SJF_METHOD  2
#define SELF_METHOD 3

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

typedef vector<JobControlBlock> JobQueue;

static bool FCFSCompare(JobControlBlock& a, JobControlBlock& b);
static bool SJFCompare(JobControlBlock& a, JobControlBlock& b);

class JobScheduling{
    public:
        JobScheduling(){
            job_queue = new vector<JobControlBlock>();
            method = 0;
        }
        JobScheduling(vector<JobControlBlock>& jobs, int method = FCFS_METHOD):method(method){
            job_queue = new vector<JobControlBlock>(jobs);
        }
        ~JobScheduling(){
            job_queue->clear();
            delete job_queue;
        }
        void scheduling(){
            if(0 == this->method){
                
            }
            switch (this->method)
            {
                case FCFS_METHOD:   FCFS();               break;
                case SJF_METHOD:    SJF();                break;
                case SELF_METHOD:   SLEF_METHOD();        break;
                default:
                    cout << "The method ID is error" <<endl;
                    return;
            }
        }
        void outputRes(){
            if(FCFS_METHOD == this->method) cout<<"FCFS Res:\n----------------------------------------" <<endl;
            else if(SJF_METHOD == this->method) cout<<"SJF Res:\n----------------------------------------" <<endl;
            else if(SELF_METHOD == this->method) cout<<"SELF Res:\n--------------------------------------" <<endl;
            cout << "ID\tWaiting Time\tCompletion Time" << endl;
            for (int i = 0; i < job_queue->size(); i++) {
                cout << job_queue->at(i).id << "\t" << job_queue->at(i).waiting_time << "\t\t" << job_queue->at(i).completion_time << endl;
            }
            cout<<"----------------------------------------"<<endl;
        }
        // 给用户提供一个可以自己排序队列的接口 然后按照用户的排序规则进行队列先后执行 达到用户自定义调度方法的方式
        bool (*self_schdule_CMP_Method)(JobControlBlock&, JobControlBlock&);
    private:
        JobQueue*    job_queue;
        int         method = 0;
        void FCFS(){
            sort(job_queue->begin(), job_queue->end(), FCFSCompare);
            int current_time = 0;
            for (int i = 0; i < job_queue->size(); i++) {
                cout << "[DEBUG  INFO](line " << __LINE__ << "): "<<current_time <<" "
                    << job_queue->at(i).arrival_time <<endl;
                current_time = max(current_time, job_queue->at(i).arrival_time);
                job_queue->at(i).waiting_time = current_time - job_queue->at(i).arrival_time;
                job_queue->at(i).completion_time = current_time + job_queue->at(i).burst_time;
                current_time = job_queue->at(i).completion_time;
                job_queue->at(i).status = Complete; // 进程完成
            }
        }
        void SJF(){
            sort(job_queue->begin(), job_queue->end(), SJFCompare);
            int current_time = 0;
            for (int i = 0; i < job_queue->size(); i++) {
                current_time = max(current_time, job_queue->at(i).arrival_time);
                job_queue->at(i).waiting_time = current_time - job_queue->at(i).arrival_time;
                job_queue->at(i).completion_time = current_time + job_queue->at(i).burst_time;
                current_time = job_queue->at(i).completion_time;
                job_queue->at(i).status = Complete; // 进程完成
            }
        }
        void SLEF_METHOD(){
            sort(job_queue->begin(), job_queue->end(), this->self_schdule_CMP_Method);
            int current_time = 0;
            for (int i = 0; i < job_queue->size(); i++) {
                
                current_time = max(current_time, job_queue->at(i).arrival_time);
                job_queue->at(i).waiting_time = current_time - job_queue->at(i).arrival_time;
                job_queue->at(i).completion_time = current_time + job_queue->at(i).burst_time;
                current_time = job_queue->at(i).completion_time;
                job_queue->at(i).status = Complete; // 进程完成
            }
        }
};

static bool FCFSCompare(JobControlBlock& a, JobControlBlock& b){
    if (a.arrival_time == b.arrival_time) {
        return a.id < b.id;
    }
    return a.arrival_time < b.arrival_time;
}

static bool SJFCompare(JobControlBlock& a, JobControlBlock& b){
    if (a.burst_time == b.burst_time) {
        return a.id < b.id;
    }
    return a.burst_time < b.burst_time;
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
    SELFscheduling->self_schdule_CMP_Method = SJFCompare;

    
    FCFSscheduling->scheduling(); FCFSscheduling->outputRes();
    SJFscheduling->scheduling(); SJFscheduling->outputRes();
    SELFscheduling->scheduling(); SELFscheduling->outputRes();
    return 0;
}
