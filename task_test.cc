#include <iostream>
#include "tasks.h"
using namespace std;

int main()
{
    Task t1, t2;
    t2.name = "task02";
    t1.name = "task1";

    Storage s("wsm");
    if(s.fail())
    {
        puts("open file failed!");
        return -1;
    }
    Tasks T(s);
    T.insert(t1);
    cout << T.insert(t1) << endl;
    T.insert(t2);
    auto v = T.sort(&Task::name);
    for(auto x : v)
        cout << x << " ";
    cout << endl;
    for(auto x : v)
        cout << T[x].name.data() << " ";
    cout << endl;
    s.cancel();
    return 0;
}