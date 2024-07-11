#ifndef SCHEDULE_TASKS_H
#define SCHEDULE_TASKS_H

#include "storage.h"
#include <vector>
#include <functional>

// one user's tasks
class Tasks
{
private:
    Storage *s;
public:
    ///constructor
    Tasks(Storage &storage);
    ///modifier
    bool insert(const Task &task);
    ///element
    Task &operator[](uint64_t taskID);
    ///select
    //general
    std::vector<uint64_t> select(std::function<bool(const Task &)> pred);
    //equal
    std::vector<uint64_t> select(const Task &);
    //member
    template<class Member>
    std::vector<uint64_t> select(Member Task:: *pointer, Member value);
    ///sort
    //general
    std::vector<uint64_t> sort(std::function<bool(const Task &, const Task &)> cmp);
    //member
    template<class Member>
    std::vector<uint64_t> sort(Member Task:: *pointer, std::function<bool(const Member &, const Member &)> cmp = std::less<Member>());
};

//inline
template<class Member>
inline std::vector<uint64_t> Tasks::select(Member Task:: *pointer, Member value)
{
    return select([&](const Task &x) -> bool
        {
            return x.*pointer == value;
        });
}

template<class Member>
inline std::vector<uint64_t> Tasks::sort(Member Task:: *pointer, std::function<bool(const Member &, const Member &)> cmp)
{
    return sort([&](const Task &x, const Task &y)
        {
            return cmp(x.*pointer, y.*pointer);
        });
}

#endif // SCHEDULE_TASKS_H