#ifndef SCHEDULE_STORAGE_H
#define SCHEDULE_STORAGE_H

#include "task.h"
#include "user.h"

#include <stddef.h> // size_t

class Storage
{
private:
    int fd; // file description
    void *mapping; // mapping address
    size_t *used; // used size
    size_t mapsize; // mapping size

    // reserve the file size to `capacity` if it is smaller; remap the memory anyway
    void reserve(size_t capacity);
public:
    /// construct/destruct
    // Default
    Storage();
    // Open a schedule with username `name`; i.e. sign in
    Storage(const char *name);
    // Create a schedule with username `name`; i.e. sign up
    Storage(const User &user);
    ~Storage();
    // whether something wrong happen
    bool fail() const;
    /// user
    // Sign up a new account
    // If the account already exists, fail() is true
    void signup(const User &user);
    // Sign in an account
    // If the account doesn't exist, fail() is true
    void signin(const char *name);
    // sign out the account
    void signout();
    // Cancel the account
    // After executing, all handler will immediately fail
    void cancel();
    // Get User fields
    User &user();
    /// task
    // Get Task array and write length into `len`
    Task *tasks(uint64_t &len);
    // Any task insertion MUST use insert_task method, to allocate
    // new memory on file; otherwise SIGSEGV may be raised
    void insert_task(const Task &task);
};

#endif // SCHEDULE_STORAGE_H