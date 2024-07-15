#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <stdint.h>
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
    remind_t();
    remind_t(tm &tm);
    //check whether it is time to remind; if it is, then let isReminded = true;
    bool check();
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
    Array<uint8_t, MAX_TAGS_PER_TASK> tags;
    Priority priority;
    Status status;
    char content[TASKCONTENT_SIZE];

    //construct
    Task();
    Task(std::string con);
};

#endif // SCHEDULE_TASK_H