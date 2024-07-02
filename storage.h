
#ifndef SCHEDULE_STORAGE_H
#define SCHEDULE_STORAGE_H

class UserSesson;
#include "task.h"
#include "account.h"
#include <stdio.h>

class UserSesson{
    int fd;
    char* mapping;
    ssize_t used; // used size
    ssize_t mapsize; // mapping size
public:
    /// Check if the session is valid
    bool valid(){return fd==-1;}
    /// Open a schedule with name `name`.
    /// If file does not exist, create a new one.
    UserSesson(const char* name);
    ~UserSesson();
    /// Cancel the account. After executing, all handler
    /// will immediately fail
    void cancel();
    /// Return its User handler
    User& user();
    /// Return its Task handler and write length into `len`
    Task* tasks(int& len);
    /// Any task insertion MUST use insert_task method, to allocate
    /// new memory on file; otherwise SIGSEGV may be raised
    void insert_task(const Task& task);
};

#endif // SCHEDULE_STORAGE_H