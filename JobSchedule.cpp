#include "JonSchedule.h"

static bool FCFSCompare(JobControlBlock& a, JobControlBlock& b);
static bool SJFCompare(JobControlBlock& a, JobControlBlock& b);

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

// JobSchduleBase::JobSchduleBase(vector<JobControlBlock>& jobs){
//     cout << "1" <<endl;
//     job_queue = new vector<JobControlBlock>(jobs);
// }

void JobSchduleBase::outputRes() {
    cout<< _algorithm_name << " Res:\n----------------------------------------" <<endl;
    cout << "ID\tWaiting Time\tCompletion Time" << endl;
    for (int i = 0; i < job_queue->size(); i++) {
        cout << job_queue->at(i).id << "\t" << job_queue->at(i).waiting_time << "\t\t" << job_queue->at(i).completion_time << endl;
    }
    cout<<"----------------------------------------"<<endl;
}

FCFSSchdule:: ~FCFSSchdule(){
    job_queue->clear();
    delete job_queue;
}
void FCFSSchdule::schduling() { FCFS(); }
void FCFSSchdule::FCFS(){
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

SJFSchdule::~SJFSchdule(){
    job_queue->clear();
    delete job_queue;
}

void SJFSchdule::schduling() { SJF(); }
void SJFSchdule::SJF(){
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
