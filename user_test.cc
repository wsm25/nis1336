#include<iostream>

#include"user.h"

using namespace  std;

int main()
{
    User my_user("Penny");
    my_user.add_tag("entertain");
    my_user.add_tag("study");
    my_user.add_tag("read");
    my_user.set_password("haha666");
    my_user.get_inform();
    bool flag = my_user.verify_password("haha666");
    if(flag)
        cout << "yes" << endl;
    else
        cout << "no" << endl;
    return 0;
}