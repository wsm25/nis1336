#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <stdint.h>
#include <time.h>
#include "array.h"
#include "consts.h"

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
    friend bool operator==(const Dates &d1, const Dates &d2);
    friend bool operator!=(const Dates &d1, const Dates &d2);
    friend bool operator<(const Dates &d1, const Dates &d2);
    friend bool operator>(const Dates &d1, const Dates &d2);
    friend bool operator<=(const Dates &d1, const Dates &d2);
    friend bool operator>=(const Dates &d1, const Dates &d2);
};

struct Task
{
    ///member type
    enum Priority { None, Low, Mid, High, };
    enum Status { Unfinished, Finished, Abort,};

    ///fields
    Array<char, TASKNAME_SIZE> name;
    time_t begin, end;
    Array<uint8_t, MAX_TAGS_PER_TASK> tags;
    //Dates dates;
    Priority priority;
    Status status;
    char content[TASKCONTENT_SIZE];
};

#endif // SCHEDULE_TASK_H