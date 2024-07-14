#include "task.h"

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
