#include "storage.h"
#include "task.h"
#include <cstdio>

int main(){
    Storage s("wsm");
    if(s.fail()){
        puts("open file failed!");
        return -1;
    }
    Task t;
    for(int i=0; i<100; i++)
        s.insert_task(t);
    s.cancel();
}