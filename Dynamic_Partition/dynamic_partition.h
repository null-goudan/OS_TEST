#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

#define FIRST_FIT 1
#define BEST_FIT  2
#define WORST_FIT 3

using namespace std;

typedef unsigned int address_mem;
struct memory_item{
    memory_item()=default;
    memory_item(int mid, address_mem start, unsigned int size, bool is_alloc = false)
            :mid(mid),start(start), end(start + size - 1) , size(size), is_alloc(is_alloc), pid(-1){}
    int mid;
    bool is_alloc;
    unsigned int size;
    // 分配空间的起始位置
    address_mem start;
    address_mem end;
    // 分配空间的作业信息
    int pid;    // 对应的进程/任务的pid
};

class memory_dynamic_patition{
    public:
        memory_dynamic_patition(unsigned int init_size);
        bool alloc(int pid, unsigned int size, int method);
        bool free(int pid);
        void showNow();
    private:
        vector<memory_item*> memory;
        int mid_cnt = 1;
};