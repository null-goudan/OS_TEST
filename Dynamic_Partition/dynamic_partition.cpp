#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

#define FIRST_FIT 1
#define BEST_FIT  2

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

vector<memory_item*> memory;
int mid_cnt = 1;

bool cmp_startadd(memory_item* a, memory_item* b){
    return a->start < b->start;
}

bool cmp_size(memory_item* a, memory_item* b){
    if(a->size == b->size) return a->start < b->start;
    return a->size < b->size;                                                     
}

bool alloc_s(int pid, unsigned int size, int method){
    for(auto i:memory){
        if(i->pid == pid) {
            cout<<"PID already alloced"<<endl;
            return false;
        }
    }
    switch (method)
    {
    case FIRST_FIT:
        sort(memory.begin(), memory.end(), cmp_startadd);   // 按照地址排序
        for(int i = 0;i<memory.size(); ++i){
            memory_item* NowMemoryItem = memory[i];
            if(!NowMemoryItem->is_alloc && NowMemoryItem->size > size){ //遇到第一个有足够空间的内存块
                // 分配空间
                memory_item* newMemoryItem = new memory_item(++mid_cnt, NowMemoryItem->start, size, true);
                newMemoryItem->pid = pid;
                // 更新内存情况
                memory_item* lastMemoryItem = new memory_item(NowMemoryItem->mid, newMemoryItem->end + 1, NowMemoryItem->size - size);
                memory.erase(memory.begin() + i); delete NowMemoryItem;
                memory.push_back(newMemoryItem); memory.push_back(lastMemoryItem);
                return true;
            }
        }
        break;
    case BEST_FIT:
    // 总是选择最小的空闲空间，所以可以直接从小到大排列
        sort(memory.begin(), memory.end(), cmp_size);   // 按照空间排序后， 就可以按序查找到最小适合的空间了
        for(int i = 0;i<memory.size(); ++i){
            memory_item* NowMemoryItem = memory[i];
            if(!NowMemoryItem->is_alloc && NowMemoryItem->size > size){ //遇到第一个有足够空间的内存块
                // 分配空间
                memory_item* newMemoryItem = new memory_item(++mid_cnt, NowMemoryItem->start, size, true);
                newMemoryItem->pid = pid;
                // 更新内存情况
                memory_item* lastMemoryItem = new memory_item(NowMemoryItem->mid, newMemoryItem->end + 1, NowMemoryItem->size - size);
                memory.erase(memory.begin() + i); delete NowMemoryItem;
                memory.push_back(newMemoryItem); memory.push_back(lastMemoryItem);
                return true;
            }
        }
        break;
    default:
        cout<<"The method name is not define"<<endl;
        return false;
    }
    cout<<"The space is not enough"<<endl;
    return false;
}   

bool free_s(int pid){
    if(memory[0]->pid == pid)        // 第一个分区的情况
    {
        // 如果第二个分区是空余分区， 合并他们(删除第二个分区，将第一个分区大小加上第二个分区的size)
        if(!memory[1]->is_alloc){
            memory[0]->size += memory[1]->size;
            memory.erase(memory.begin() + 1);
        }
        memory[0]->is_alloc = false;
        memory[0]->pid = -1;
        return true;
    }
    for(int i = 1;i<memory.size() - 1; ++i){
        // 中间分区的情况
        if(memory[i]->pid == pid){
            // 1. 前后都是空闲分区 
            if(!memory[i-1]->is_alloc && !memory[i+1]->is_alloc){
                // 将最前面的的分区的大小合并
                memory[i-1]->size = memory[i-1]->size + memory[i]->size + memory[i+1]->size;
                // 将后面两个进行删除
                memory.erase(memory.begin() + i, memory.begin() + (i+1));
                return true;
            }
            // 2. 前面的分区是空闲区   
            if(!memory[i-1]->is_alloc){
                // 将前面的分区和现在的分区进行合并
                memory[i-1]->size += memory[i]->size;
                memory.erase(memory.begin() + i);
            }
            // 3. 后面的分区是空闲区
            if(!memory[i+1]->is_alloc){
                // 将后面的分区合并入现在的分区
                memory[i]->size += memory[i+1]->size;
                memory.erase(memory.begin() + i + 1);
                memory[i]->pid = -1;
                memory[i]->is_alloc = false;
            }
            // 4. 只有匹配上的分区是空闲区
            memory[i]->pid = -1;
            memory[i]->is_alloc = false;
        }
    }
    if(memory[memory.size() - 1]->pid == pid)        //最后一个分区的情况
    {
        // 如果倒数第二个分区是空余分区， 合并他们(删除最后一个个分区，将倒数第二个分区大小加上最后一个分区的size)
        if(!memory[memory.size() - 2]->is_alloc){
            memory[memory.size() - 2]->size += memory[memory.size() - 1]->size;
            memory.erase(memory.end() - 1);
        }
        memory[memory.size() - 1]->is_alloc = false;
        memory[memory.size() - 1]->pid = -1;
        return true;
    }
    return false;
}

void showmemory(){
    sort(memory.begin(), memory.end(), cmp_startadd); // 方便显示 
    printf("%4s\t%14s\t%14s\t%5s\t%4s\t%5s\n", "mid", "start address", "end address", "size", "pid", "free?");
    for(auto i : memory){
        printf("%4d\t%14#x\t%14#x\t%5d\t%4d\t%5s\n", 
        // printf("%4d\t%14d\t%14d\t%5d\t%4d\t%5s\n", 
                i->mid, i->start, i->end, i->size, i->pid, (i->is_alloc? "No" : "Yes"));
    }
}

int main(){
    // 初始化,给初始分区分配空间
    unsigned size_init;
    cout<<"Input size of init partition:"<<endl;
    cin>>size_init;
    memory_item* first_item = new memory_item(1, 0, size_init);
    memory.push_back(first_item);
    showmemory();
    alloc_s(1, 512, BEST_FIT);
    showmemory();
    alloc_s(2, 128, BEST_FIT);
    showmemory();
    alloc_s(3, 128, BEST_FIT);
    showmemory();
    free_s(2);
    showmemory();
    alloc_s(4, 64, BEST_FIT);
    showmemory();
    free_s(1);
    showmemory();
    alloc_s(1, 64, BEST_FIT);
    showmemory();
    return 0;
}