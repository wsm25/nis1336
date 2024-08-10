#include "task.h"
#include <iostream>
#include <iomanip>

remind_t::remind_t(time_t tm): t(tm), isReminded(false) {}

bool remind_t::check() const
{
    return (t <= time(NULL) && !isReminded);
}

Task::Task(): name("new task"), 
    begin(time(NULL)), end(time(NULL)), remind(), 
    priority(None), status(Unfinished), 
    tags(), content() {}

void Task::showtask() const
{
    std::cout << std::left << std::setw(TASKNAME_SIZE) << name;
    std::cout << std::put_time(localtime(&begin), "%Y/%m/%d-%T  ");
    std::cout << std::put_time(localtime(&end), "%Y/%m/%d-%T  ");
    std::cout << std::put_time(localtime(&remind.t), "%Y/%m/%d-%T  ");
    switch(priority)
    {
    case None: std::cout << std::left << std::setw(PRINAME_SIZE) << "none"; break;
    case Low: std::cout << std::left << std::setw(PRINAME_SIZE) << "low"; break;
    case Mid: std::cout << std::left << std::setw(PRINAME_SIZE) << "mid"; break;
    case High: std::cout << std::left << std::setw(PRINAME_SIZE) << "high"; break;
    }
    switch(status)
    {
    case Unfinished: std::cout << std::left << std::setw(STATNAME_SIZE) << "Unfin"; break;
    case Finished: std::cout << std::left << std::setw(STATNAME_SIZE) << "Fin"; break;
    case Abort: std::cout << std::left << std::setw(STATNAME_SIZE) << "Abort"; break;
    }
    std::cout << std::left << std::setw(TAGNAME_SIZE) << tags << std::endl;
    std::cout << "\t" << content << std::endl;
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
