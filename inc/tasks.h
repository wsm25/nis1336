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
    Task *data();
    const Task *data() const;
    uint64_t size() const;
    Task &operator[](uint64_t taskID);
    Task &at(uint64_t taskID);
    ///select
    //general
    std::vector<uint64_t> select(std::function<bool(const Task &)> pred) const;
    //equal
    std::vector<uint64_t> select(const Task &) const;
    //member
    template<class Member>
    std::vector<uint64_t> select(Member Task:: *pointer, Member value) const;
    ///sort
    //general
    std::vector<uint64_t> sort(std::function<bool(const Task &, const Task &)> cmp) const;
    //member
    template<class Member>
    std::vector<uint64_t> sort(Member Task:: *pointer, 
        std::function<bool(const Member &, const Member &)> cmp = std::less<Member>()) const;
};

//inline
template<class Member>
inline std::vector<uint64_t> Tasks::select(Member Task:: *pointer, Member value) const
{
    return select([&](const Task &x) -> bool
        {
            return x.*pointer == value;
        });
}

template<class Member>
inline std::vector<uint64_t> Tasks::sort(Member Task:: *pointer, 
    std::function<bool(const Member &, const Member &)> cmp) const
{
    return sort([&](const Task &x, const Task &y)
        {
            return cmp(x.*pointer, y.*pointer);
        });
}

#endif // SCHEDULE_TASKS_H