#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <stdint.h>
#include <time.h>
#include "array.h"
#include "consts.h"

struct Task
{
    ///member type
    enum Priority { None, Low, Mid, High, };
    enum Status { Unfinished, Finished, Abort,};

    ///fields
    Array<char, TASKNAME_SIZE> name;
    time_t begin, end, remind;
    Array<uint8_t, MAX_TAGS_PER_TASK> tags;
    Priority priority;
    Status status;
    char content[TASKCONTENT_SIZE];
};

#endif // SCHEDULE_TASK_H