
#ifndef SCHEDULE_STORAGE_H
#define SCHEDULE_STORAGE_H

#include <stdio.h>

class Storage;
#include "task.h"
#include "user.h"

class Storage {
    int fd;
    char* mapping;
    ssize_t used; // used size
    ssize_t mapsize; // mapping size
public:
    /// Check if the session is valid
    bool valid(){return fd==-1;}
    /// Open a schedule with name `name`.
    /// If file does not exist, create a new one.
    Storage(const char* name);
    ~Storage();
    /// Cancel the account. 
    /// After executing, all handler will immediately fail
    void cancel();
    /// Get User fields
    User& user();
    /// Get Task array and write length into `len`
    Task* tasks(int& len);
    /// Any task insertion MUST use insert_task method, to allocate
    /// new memory on file; otherwise SIGSEGV may be raised
    void insert_task(const Task& task);
};

#endif // SCHEDULE_STORAGE_H