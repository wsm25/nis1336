#include "tasks.h"
#include <algorithm>

Tasks::Tasks(Storage &stor): s(&stor) {}

bool Tasks::insert(const Task &t)
{
    if(select(t).empty())
    {
        s->insert_task(t);
        return true;
    }
    else
        return false;
}

Task &Tasks::operator[](uint64_t ID)
{
    uint64_t len;
    Task *arr = s->tasks(len);

    if(ID >= len) throw std::out_of_range("Task &Tasks::operator[](uint64_t)");
    return arr[ID];
}

std::vector<uint64_t> Tasks::select(std::function<bool(const Task &)> pred)
{
    uint64_t len;
    Task *arr = s->tasks(len);

    std::vector<uint64_t> v;
    for(uint64_t i = 0; i < len; ++i)
        if(pred(arr[i]))
            v.push_back(i);
    return v;
}

std::vector<uint64_t> Tasks::select(const Task &t)
{
    return select([&t](const Task &x) -> bool
        {
            return x.begin == t.begin;
        });
}

std::vector<uint64_t> Tasks::sort(std::function<bool(const Task &, const Task &)> cmp)
{
    uint64_t len;
    Task *arr = s->tasks(len);

    std::vector<uint64_t> v;
    for(uint64_t i = 0; i < len; ++i)
        v.push_back(i);
    std::sort(v.begin(), v.end(), [&](uint64_t x, uint64_t y) -> bool
        {
            return cmp(arr[x], arr[y]);
        });
    return v;
}
