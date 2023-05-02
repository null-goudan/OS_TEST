#include "JobScheduling.h"

bool FCFSCompare(JobControlBlock& a, JobControlBlock& b){
    if (a.arrival_time == b.arrival_time) {
        return a.id < b.id;
    }
    return a.arrival_time < b.arrival_time;
}

bool SJFCompare(JobControlBlock& a, JobControlBlock& b){
    if (a.burst_time == b.burst_time) {
        return a.id < b.id;
    }
    return a.burst_time < b.burst_time;
}

void JobScheduling::JoinNewJob(JobControlBlock& newJob){
    this->job_queue->push_back(newJob);
}

void JobScheduling::scheduling(){
    // sort(job_queue->begin(), job_queue->end(), FCFSCompare);
    int current_time = 0;
    do{
        // 将作业队列中的处在就绪状态的作业装入等待队列中，等待调度
        if(!job_queue->empty()){    /*就是检测是否装入了新的作业,或者非空时检测是否有新就绪的作业
                                    (这里做成多线程的话，能够更加模拟出真实的作业加入并检测的效果， 这里如果没有即时插入的话，那就有点鸡肋了
                                    tip：不要求做，那就简单模拟一下，意思意思就得了) */
            for(int i =0;i < job_queue->size(); ++i){
                if(Ready == job_queue->at(i).status){
                    wait_queue.push_back(job_queue->at(i));
                    // 在原始队列中删除对应的作业
                    job_queue->erase(job_queue->begin() + i);
                    i--;
                } //已就绪的加入等待队列,等待调度
            }
        }
        // 按照规则对等待队列排序
        switch (this->method)
        {
            case FCFS_METHOD:   sort(wait_queue.begin(), wait_queue.end(), FCFSCompare);               break;
            case SJF_METHOD:    sort(wait_queue.begin(), wait_queue.end(), SJFCompare);                break;
            case SELF_METHOD:   sort(wait_queue.begin(), wait_queue.end(), cmpMethod);                 break;
            default:
                cout << "The method ID is error , can't schdule" <<endl;
                return;
        }
        // 取等待序列第一个进行调度
        // 现在时间
        current_time = max(current_time, wait_queue[0].arrival_time); // 只用直接将第一个作业的到达时间替换就行
        // ↓ 计算等待时间 (鄙人觉得，每次调度的时候才会计算/定型，而不是每次都会遍历整个队列，所以如是写，真正查询的时候那些还在等待队列可以直接写上当前时间，也不失真实性)
        wait_queue[0].waiting_time = current_time - wait_queue[0].arrival_time;
        // 运行执行等待队列第一个
        wait_queue[0].status = Run;
        // 由于实验条件中，该作业被直接完成（没有时间片概念），省略运行过程, 如果不省略的话，那就是时间片轮换运行代码
        // run(wait_queue[0].task)  task 应为真正要执行的任务，为一个指针，指向运行工作的代码空间的某地址，这个地址存放着下一步要执行的指令 占用CPU一段时间后更新该地址
        // ↓ 计算完成时间(周转时间) 由于直接完成了(如果是时间片的话，那么就得加判断了，判断要执行的代码指针指向的地址是否是尾)，那就直接可以计算完成时间了， 而且给出了执行时间所以直接加就行了 
        wait_queue[0].completion_time = current_time + wait_queue[0].burst_time;
        // 更新现在时间
        current_time = current_time + wait_queue[0].burst_time;
        // 将该作业变更为完成状态 加入完成队列 并重新装入最开始的队伍管理序列(方便最后输出) [进程调度完成]
        wait_queue[0].status = Complete;
        complete_queue.push_back(wait_queue[0]);
        job_queue->push_back(wait_queue[0]);
        wait_queue.erase(wait_queue.begin());

        // 应实验要求，在这输出当前调度的状态(每次调度完)
        ShowNowJobs();

    }while(!wait_queue.empty());
}

void JobScheduling::outputRes(){
    if(FCFS_METHOD == this->method) cout<<"FCFS Res:\n----------------------------------------" <<endl;
    else if(SJF_METHOD == this->method) cout<<"SJF Res:\n----------------------------------------" <<endl;
    else if(SELF_METHOD == this->method) cout<<"SELF Res:\n--------------------------------------" <<endl;
    cout << "ID\tWaiting Time\tCompletion Time" << endl;
    for (int i = 0; i < job_queue->size(); i++) {
        cout << job_queue->at(i).id << "\t" << job_queue->at(i).waiting_time << "\t\t" << job_queue->at(i).completion_time << endl;
    }
    cout<<"----------------------------------------"<<endl;
}

void JobScheduling::ChangeSelfMethod(bool (*method)(JobControlBlock&, JobControlBlock&)){
    this->cmpMethod = method;
}

// 输出当前调度状态（所有作业）
void JobScheduling::ShowNowJobs(){
    // 等待队列
    cout<<"等待队列:"<<endl;
    for(auto i: wait_queue){

    }
    // 完成队列
    cout<<"完成队列:"<<endl;
    for(auto i: complete_queue){

    }
}

