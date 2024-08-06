#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <time.h>
#include "array.h"
#include "consts.h"

struct remind_t
{
    ///fields
    time_t t;
    bool isReminded;

    ///function
    //construct
    remind_t(time_t tm = time(NULL));
    //check whether it is time to remind; if it is, then let isReminded = true;
    bool check() const;
    //compare
    friend bool operator==(const remind_t &t1, const remind_t &t2);
    friend bool operator!=(const remind_t &t1, const remind_t &t2);
    friend bool operator<(const remind_t &t1, const remind_t &t2);
    friend bool operator>(const remind_t &t1, const remind_t &t2);
    friend bool operator<=(const remind_t &t1, const remind_t &t2);
    friend bool operator>=(const remind_t &t1, const remind_t &t2);
};

struct Task
{
    ///member type
    enum Priority { None, Low, Mid, High, };
    enum Status { Unfinished, Finished, Abort,};

    ///fields
    Array<char, TASKNAME_SIZE> name;
    time_t begin, end;
    remind_t remind;
    Priority priority;
    Status status;
    Array<char, TAGNAME_SIZE> tags;
    char content[TASKCONTENT_SIZE];

    //construct
    Task();
    void showtask() const;
};

#endif // SCHEDULE_TASK_H