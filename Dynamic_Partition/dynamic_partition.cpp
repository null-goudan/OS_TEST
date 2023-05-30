#include "dynamic_partition.h"

bool cmp_startadd(memory_item* a, memory_item* b){
    return a->start < b->start;
}

bool cmp_size_min(memory_item* a, memory_item* b){
    if(a->size == b->size) return a->start < b->start;
    return a->size < b->size;                                                     
}

bool cmp_size_max(memory_item* a, memory_item* b){
    if(a->size == b->size) return a->start < b->start;
    return a->size > b->size;                                                     
}

bool memory_dynamic_patition::alloc(int pid, unsigned int size, int method){
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
        sort(memory.begin(), memory.end(), cmp_size_min);   // 按照空间排序后， 就可以按序查找到最小适合的空间了
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
    case WORST_FIT: // 最坏适应算法
    // 选择最长的空闲空间，所以直接按照空间从大到小排列
        sort(memory.begin(), memory.end(), cmp_size_max);   // 按照空间排序后， 就可以按序查找到最大的空间了
        // 最坏适应算法找到的第一个就是最大的空间，如果这个空间的大小不足够的话那么就直接返回空间不够
        for(int i = 0;i<memory.size(); ++i){
            memory_item* NowMemoryItem = memory[i];
            if(!NowMemoryItem->is_alloc){ //遇到第一个空闲的内存块
                if(NowMemoryItem->size >= size){ // 如果第一个空间足够
                    // 分配空间
                    memory_item* newMemoryItem = new memory_item(++mid_cnt, NowMemoryItem->start, size, true);
                    newMemoryItem->pid = pid;
                    // 更新内存情况
                    memory_item* lastMemoryItem = new memory_item(NowMemoryItem->mid, newMemoryItem->end + 1, NowMemoryItem->size - size);
                    memory.erase(memory.begin() + i); delete NowMemoryItem;
                    memory.push_back(newMemoryItem); memory.push_back(lastMemoryItem);
                    return true;
                }else{  // 第一个空间不够直接提示空间不够
                    cout<<"The space is not enough"<<endl;
                    return false;
                }
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

bool memory_dynamic_patition::free(int pid){
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
            return true;
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

void memory_dynamic_patition::showNow(){
    sort(memory.begin(), memory.end(), cmp_startadd); // 方便显示 
    printf("%4s\t%14s\t%14s\t%5s\t%4s\t%5s\n", "mid", "start address", "end address", "size", "pid", "free?");
    for(auto i : memory){
        printf("%4d\t%14x\t%14x\t%5d\t%4d\t%5s\n", 
        // printf("%4d\t%14d\t%14d\t%5d\t%4d\t%5s\n", 
                i->mid, i->start, i->end, i->size, i->pid, (i->is_alloc? "No" : "Yes"));
    }
}

memory_dynamic_patition::memory_dynamic_patition(unsigned int init_size){
    memory_item* first_item = new memory_item(1, 0, init_size);
    memory.push_back(first_item);
    this->mid_cnt = 0;
}