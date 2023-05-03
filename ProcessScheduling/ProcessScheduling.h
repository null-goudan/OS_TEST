#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum ProcessStatus {
    Ready = 0,          // 就绪态
    Run = 1,            // 工作态
    Complete = 2,       // 完成
    // 这里可能还有其它状态，比如阻塞，但是本次实验不要求做，就懒省事不做啦
};

struct ProcessControlBlock
{
    ProcessControlBlock() = default;
    ProcessControlBlock(int pid, ProcessStatus status, int arrival_time, int burst_time, int priority, int waiting_time = 0,
        int completion_time = -1)
        :pid(pid), status(status), arrival_time(arrival_time), burst_time(burst_time), waiting_time(waiting_time)
        , completion_time(completion_time), priority(priority), next_task(nullptr), prev_task(nullptr) {}

    int             pid;                        // 进程ID(唯一标识)
    ProcessStatus   status;                     // 进程状态
    // 进程调度信息
    int             arrival_time;               // 到达时间
    int             burst_time;                 // 需要执行时间
    int             waiting_time;               // 等待时间
    int             running_time = 0;           // 运行时间
    int             completion_time;            // 完成时间
    int             priority;                   // 进程优先级
    double          turnaround_time = 0.0;      // 周转时间
    bool            flag_isInqueue = false;     // 是否已经加入就绪队列标志位
    // 进程正文、数据及栈在内存区域的指针
    void*           content;
    void*           data_area;
    void*           run_now_ptr;
    int             cur_code_line = 1;              // 当前代码行
    // 其实这里还有CPU的相关信息 寄存器信息等
    // ....

    // 进程间链接指针(相当于做了链表了)
    ProcessControlBlock* next_task, * prev_task; // 进程PCB双向链表的前后项指针

    // 指明进程家族间的关系，分别为指向祖父进程、父进程、子进程以及新老进程的指针。（实验不做要求就不具体用了）
    // struct ProcessControlBlock *p_opptr,*p_pptr,*p_cptr,*p_ysptr,*p_ptr     
};

typedef vector<ProcessControlBlock*> ProcessesQueue;

class ProcessScheduling {  // 使用动态优先级的调度算法对象
public:
    ProcessScheduling() = default;
    ProcessScheduling(vector<ProcessControlBlock>& processes);
    void startScheduling();
    void outputRes();
    void showProcesses();
    void AddProcess(ProcessControlBlock newProcess);
private:
    ProcessControlBlock* processes = nullptr;        // 进程表
    int plist_size = 0;                               // 进程表项数

    ProcessesQueue        wait_queue;                 // 就绪队列
    // ProcessesQueue     sleep_queue;                // 阻塞队列  (应该还有阻塞队列，实验不做要求)

    double                average_time = 0.0;         // 平均周转时间
};

static bool compareFunc(ProcessControlBlock* a, ProcessControlBlock* b);
