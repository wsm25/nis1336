#ifndef SCHEDULE_STORAGE_H
#define SCHEDULE_STORAGE_H
#include "task.h"
#include "account.h"

class UserSesson{
public:
    /// Open a schedule with name `name`.
    /// If file does not exist, create a new one.
    UserSesson(char* name);
    /// Cancel the account. After executing, all handler
    /// will immediately fail
    void cancel();
    /// Return its User handler
    User& user();
    /// Return its Task handler and write length into `len`
    Task* tasks(int& len);
    /// Any task insertion MUST use insert_task method, to allocate
    /// new memory on file; otherwise SIGSEGV may be raised
    void insert_task(Task task);
};

#endif // SCHEDULE_STORAGE_H