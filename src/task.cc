#include "task.h"

remind_t::remind_t(time_t tm): t(tm), isReminded(false) {}

bool remind_t::check() const
{
    return (t <= time(NULL) && !isReminded);
}

Task::Task(): name("new task"), 
    priority(None), status(Unfinished), 
    begin(time(NULL)), end(time(NULL)), remind(), 
    content() {}

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
