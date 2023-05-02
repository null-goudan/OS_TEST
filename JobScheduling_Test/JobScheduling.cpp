#include "JobScheduling.h"

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

void JobScheduling::scheduling(){
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
    this->self_schdule_CMP_Method = method;
}

void JobScheduling::FCFS(){
    sort(job_queue->begin(), job_queue->end(), FCFSCompare);
    int current_time = 0;
    for (int i = 0; i < job_queue->size(); i++) {
        // cout << "[DEBUG  INFO](line " << __LINE__ << "): "<<current_time <<" "
        //     << job_queue->at(i).arrival_time <<endl;
        current_time = max(current_time, job_queue->at(i).arrival_time);
        job_queue->at(i).waiting_time = current_time - job_queue->at(i).arrival_time;
        job_queue->at(i).completion_time = current_time + job_queue->at(i).burst_time;
        current_time = job_queue->at(i).completion_time;
        job_queue->at(i).status = Complete; // 进程完成
    }
}

void JobScheduling::SJF(){
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

void JobScheduling::SLEF_METHOD(){
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


