#include "storage.h"
#include "task.h"
#include <cstdio>

int main() {
    Storage s1("wsm");
    if(s1.fail()) {
        puts("open file failed!");
        s1.signup(User("wsm"));
        if(s1.fail()) return -1;
        else puts("successfully signup!");
    }
    Task t;
    for(int i = 0; i < 32; i++)
        s1.insert_task(t);

    Storage s2((User)"wsm");
    if(s2.fail()) {
        puts("username already exist");
        s2.signin("wsm");
        if(s2.fail()) return -1;
        else puts("successfully login!");
    }
    for(int i = 0; i < 32; i++)
        s2.insert_task(t);
    s2.cancel();
}