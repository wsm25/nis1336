#include "task.h"
#include <iostream>
#include <iomanip>

remind_t::remind_t(time_t tm): t(tm), isReminded(false) {}

bool remind_t::check() const
{
    return (t <= time(NULL) && !isReminded);
}

Task::Task(): name("new task"), 
    priority(None), status(Unfinished), 
    begin(time(NULL)), end(time(NULL)), remind(), 
    content() {}

void Task::showtask()
{
    char time[20];
    std::cout << std::left << std::setw(TASKNAME_SIZE) << name;

    strftime(time, 20, "%Y/%m/%d-%T", localtime(&begin));
    std::cout << std::left << std::setw(20) << time;
    strftime(time, 20, "%Y/%m/%d-%T", localtime(&end));
    std::cout << std::left << std::setw(20) << time;
    strftime(time, 20, "%Y/%m/%d-%T", localtime(&remind.t));
    std::cout << std::left << std::setw(20) << time;

    switch(priority)
    {
    case None: std::cout << std::left << std::setw(5) << "none"; break;
    case Low: std::cout << std::left << std::setw(5) << "low"; break;
    case Mid: std::cout << std::left << std::setw(5) << "mid"; break;
    case High: std::cout << std::left << std::setw(5) << "high"; break;
    }
    switch(status)
    {
    case Unfinished: std::cout << std::left << std::setw(6) << "Unfin"; break;
    case Finished: std::cout << std::left << std::setw(6) << "Fin"; break;
    case Abort: std::cout << std::left << std::setw(6) << "Abort"; break;
    }
    std::cout << std::endl;
    std::cout << tags << " " << content << std::endl;
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
