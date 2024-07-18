#include "tasks.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

Tasks::Tasks(Storage &stor): s(&stor) {}

bool Tasks::insert(const Task &t)
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    if(select(t).empty())
    {
        s->insert_task(t);
        return true;
    }
    else
    {
        std::cerr << RED << "addtask: Task repeats" << RESET << std::endl;
        return false;
    }
}

Task *Tasks::data()
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);
    return arr;
}

const Task *Tasks::data() const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);
    return arr;
}

uint64_t Tasks::size() const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);
    return len;
}

Task &Tasks::operator[](uint64_t ID)
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);

    if(ID >= len) throw std::out_of_range("Task &Tasks::operator[](uint64_t)");
    return arr[ID];
}

Task &Tasks::at(uint64_t ID)
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);

    if(ID >= len) throw std::out_of_range("Task &Tasks::operator[](uint64_t)");
    return arr[ID];
}

std::vector<uint64_t> Tasks::select(std::function<bool(const Task &)> pred) const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    uint64_t len;
    Task *arr = s->tasks(len);

    std::vector<uint64_t> v;
    for(uint64_t i = 0; i < len; ++i)
        if(pred(arr[i]))
            v.push_back(i);
    return v;
}

std::vector<uint64_t> Tasks::select(const Task &t) const
{
    return select([&t](const Task &x) -> bool
        {
            return x.begin == t.begin;
        });
}

std::vector<uint64_t> Tasks::sort(std::function<bool(const Task &, const Task &)> cmp) const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
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

