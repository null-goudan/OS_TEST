#include "ProcessScheduling.h"

string statusName[3] = {
    "Ready", "Runing", "Completed"
};

bool compareFunc(ProcessControlBlock* a, ProcessControlBlock* b) {
    if (a->priority == b->priority) return a->pid < b->pid;
    return a->priority < b->priority;         // 优先级小的在前
}

void ProcessScheduling::AddProcess(ProcessControlBlock process) {
    if (0 == this->plist_size) {
        this->processes = new ProcessControlBlock(process);
    }
    else {
        ProcessControlBlock* pmove = this->processes;
        while (nullptr != pmove->next_task) pmove = pmove->next_task;
        pmove->next_task = new ProcessControlBlock(process);
        pmove->next_task->prev_task = pmove;
    }
    ++this->plist_size;
}

ProcessScheduling::ProcessScheduling(vector<ProcessControlBlock>& processes) {
    this->plist_size = 0;
    for (auto i : processes) {
        AddProcess(i);
    }
}

void ProcessScheduling::showProcesses() {
    cout << "Now Process Status:           now processes number:" << this->plist_size << endl;
    cout << "pid\tstatus\t\tarrival_time\t\tburst_time\t\tpriority\t\tcompletion_time" << endl;
    ProcessControlBlock* pmove = this->processes;
    if (nullptr == this->processes) return;
    while (nullptr != pmove) {
        cout << pmove->pid << "\t" << statusName[pmove->status] << "\t\t"
            << pmove->arrival_time << "\t\t" << pmove->burst_time << "\t\t"
            << pmove->priority << "\t\t" << pmove->completion_time << endl;
        pmove = pmove->next_task;
    }
}

void ProcessScheduling::startScheduling() {
    cout << "Start Scheduling: " << endl;
    cout << "Initial Process Status:" << endl;
    cout << "---------------------------------" << endl;
    showProcesses();
    int current_time = 1;
    do {
        // 遍历进程表查询并向等待队列里面加入没有加入等待队列的就绪的进程
        ProcessControlBlock* pmove = this->processes;
        while (nullptr != pmove) {
            if (Ready == pmove->status && !pmove->flag_isInqueue) {
                if (pmove->arrival_time <= current_time) // 注意到达时间 只有当到达时间等于该时间且或者小于当前时间，才会加入等待队列 相当于进程瞬时加入的感觉
                    this->wait_queue.push_back(pmove);
            }
            pmove = pmove->next_task;
        }
        // 对等待队列进行优先级排序 优先级数小的优先级高
        sort(wait_queue.begin(), wait_queue.end(), compareFunc);
        // * 取出第一个进程进行调度
        ProcessControlBlock* nowRunProcess = wait_queue[0];
        wait_queue.erase(wait_queue.begin());
        // 将进程标记为运行态
        nowRunProcess->status = Run;
        // 进程运行一个单位时间(时间片)
        // 假设一个时间片只运行一行代码(或一行指令)
        cout << "Pid: " << nowRunProcess->pid << " run its code line " << nowRunProcess->cur_code_line << endl;
        /*
            模拟时间片运行的伪代码
            run(nowRunProcess.contont, nowRunProcess.data_area);
            nowRunProcess.run_now_ptr++;
        */
        nowRunProcess->cur_code_line++;
        // 运行一个时间刻度，更新此进程的优先级，运行时间, 等待时间等
        nowRunProcess->running_time++;
        // 时间片运行完的判断，是否进行完毕 ，这个条件可以是需要服务时间和执行时间是否相等，也可以是进程控制块中的指令指向的地址是否是代码底
        if (nowRunProcess->running_time == nowRunProcess->burst_time) // 运行完毕
        {
            nowRunProcess->completion_time = current_time + 1; // 运行完毕时间
            nowRunProcess->status = Complete;
            // 计算完成时的周转时间
            nowRunProcess->turnaround_time = nowRunProcess->waiting_time + nowRunProcess->completion_time;
        }
        else {  // 没有运行完毕
            nowRunProcess->priority++; // 加一的话就是进程优先级减一
            // 对就绪队列中的所有进程等待时间进行更新
            for (auto i : wait_queue) i->waiting_time++;
            // 将现在没运行完的进程重新加入就绪队列
            wait_queue.push_back(nowRunProcess);
        }
        current_time++;
        showProcesses();
    } while (!wait_queue.empty());
}