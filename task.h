#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H
#include <cstdint>
#include <vector>
#include "storage.h"
#include "consts.h"

typedef uint64_t TaskId;
typedef int TagId;

struct Time {
    uint8_t hour, minute, second;
};

struct Date {
    uint16_t year;
    uint8_t month, day;
};

// TODO: Dates
// Dates for a task, including
// - one off
// - once per day from .. to ..
// - once per week from .. to .. on specific weekday
// - list of days
// - etc.

class Dates {
    // Supposed implement: tagged union
    enum DatesType {
        ONEOFF,
        // ...
    };
    struct OneOff {
        Date date;
    };
    struct DateList {
        int length;
        Date date[8];
    };
    // ...
    DatesType type;
    union DatesRaw {
        OneOff oneoff;
        DateList datelist;
        // ...
    }raw;
public:
    /// returns whether the date set contains given date
    bool contains(const Date&);
};

struct Task{
    enum Priority {
        Default,
        Low,
        Mid,
        High,
    };
    enum Status {
        Unfinished,
        Finished,
        Abort,
    };
    // fields
    char name[TASKNAME_SIZE]; // as file name
    Time begin, end;
    unsigned tagscount;
    TagId tags[MAX_TAGS_PER_TASK];
    Dates dates;
    Priority priority;
    Status status;
    char content[TASKCONTENT_SIZE];
};

// TODO: supported filter
struct Filter {};

// All tasks for specific user
class Tasks {
    Storage* sesson;
public:
    void insert(Task);
    std::vector<Task&> select(Filter);
    void edit(TaskId, Task);
};


#endif // SCHEDULE_TASK_H