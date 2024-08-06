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
    size_t len;
    Task *arr = s->tasks(len);
    return arr;
}

const Task *Tasks::data() const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    Task *arr = s->tasks(len);
    return arr;
}

size_t Tasks::size() const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    s->tasks(len);
    return len;
}

Task &Tasks::operator[](size_t ID)
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    Task *arr = s->tasks(len);

    if(ID >= len) throw std::out_of_range("Task &Tasks::operator[](size_t)");
    return arr[ID];
}

Task &Tasks::at(size_t ID)
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    Task *arr = s->tasks(len);

    if(ID >= len) throw std::out_of_range("Task &Tasks::operator[](size_t)");
    return arr[ID];
}

std::vector<size_t> Tasks::select(std::function<bool(const Task &)> pred) const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    Task *arr = s->tasks(len);

    std::vector<size_t> v;
    for(size_t i = 0; i < len; ++i)
        if(pred(arr[i]) && arr[i].status != Task::Abort)
            v.push_back(i);
    return v;
}

std::vector<size_t> Tasks::select(const Task &t) const
{
    return select([&t](const Task &x) -> bool
        {
            return x.begin == t.begin;
        });
}

std::vector<size_t> Tasks::sort(std::function<bool(const Task &, const Task &)> cmp) const
{
    if(s->fail()) throw std::runtime_error("Out of memory");
    size_t len;
    Task *arr = s->tasks(len);

    std::vector<size_t> v;
    for(size_t i = 0; i < len; ++i)
        if(arr[i].status != Task::Abort)
            v.push_back(i);
    std::sort(v.begin(), v.end(), [&](size_t x, size_t y) -> bool
        {
            return cmp(arr[x], arr[y]);
        });
    return v;
}

