#include "storage.h"
#include "task.h"

int main(){
    Storage s("wsm");
    Task t;
    for(int i=0; i<100; i++)
        s.insert_task(t);
    s.cancel();
}