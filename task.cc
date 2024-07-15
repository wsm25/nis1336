#include "task.h"
#include<cstring>
#include<iostream>

remind_t::remind_t(): t(time(NULL)), isReminded(false) {}
remind_t::remind_t(tm &tm): t(mktime(&tm)), isReminded(false) {}

bool remind_t::check()
{
    if(t <= time(NULL) && !isReminded)
    {
        isReminded = true;
        return true;
    }
    else
        return false;
}

Task::Task():priority(None),status(Unfinished),begin(time(NULL)),end(time(NULL))
{
    content[0] = 'N';content[1] = 'O';content[2] = 'N';content[3] = 'E';content[4] = '\0';
}
Task::Task(std::string con):priority(None),status(Unfinished),begin(time(NULL)),end(time(NULL))
{
    const char* Con = con.c_str();
    if(std::strlen(Con) >= TASKCONTENT_SIZE)
    {
        std::cerr << "Length is out of range" << std::endl;
    }
    int i = 0;
    for (; Con[i] != '\0' && i < TASKCONTENT_SIZE; ++i)
    {
        content[i] = Con[i];
    }
    content[i] = '\0';
}

bool operator==(const remind_t &t1, const remind_t &t2)
{
    return t1.t == t2.t;
}


bool operator!=(const remind_t &t1, const remind_t &t2)
{
    return !(t1 == t2);
}

bool operator<(const remind_t &t1, const remind_t &t2)
{
    return t1.t < t2.t;
}

bool operator>(const remind_t &t1, const remind_t &t2)
{
    return t2 < t1;
}

bool operator<=(const remind_t &t1, const remind_t &t2)
{
    return !(t2 < t1);
}

bool operator>=(const remind_t &t1, const remind_t &t2)
{
    return !(t1 < t2);
}
