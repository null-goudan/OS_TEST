#include "dynamic_partition.h"

int menu(){
    int i = 0;
    cout<<"select operation:\n\t1. Process memory portition\n\t2. Free the memory of Process\n\t3. Show the status of memory" <<endl;
    cin>>i;
    return i;
}

int the_method_patition(){
    int i = 0;
    cout<<"1. First Fit 2. Best Fit 3. Worst Fit"<<endl;
    cin>>i;
    return i;
}

int main(){
    unsigned int init_size;
    cout<<"Input the size of init memory:"<<endl;
    cin>>init_size;
    memory_dynamic_patition* partition = new memory_dynamic_patition(init_size);
    int select_menu = 0, choose_method = 0;
    int pid = -1; unsigned int size_this = 0;
    while(select_menu = menu()){
        switch (select_menu)
        {
        case 1:
            choose_method = the_method_patition();
            cout<<"Input the pid:"; cin>>pid;
            cout<<"Input the size of the need:"; cin>>size_this;
            if(partition->alloc(pid, size_this, choose_method)){
                cout<<"The process " << pid << " is alloc "<<size_this<<endl;
                cout<<"Now memory:" <<endl;
                partition->showNow();
            }else cout<<"Alloc Failed"<<endl;
            break;
        case 2:
            cout<<"Input the pid:"; cin>>pid;
            if(partition->free(pid)) {
                cout <<"The process "<<  pid <<"is free."<<endl;
                cout<<"Now memory:" <<endl;
                partition->showNow();
            }
            else cout<<"Free process Failed"<<endl;
            break;
        case 3:
            partition->showNow();
            break;
        default:
            cout<<"Chosee Again!"<<endl;
            break;
        }
    }
    return 0;
}