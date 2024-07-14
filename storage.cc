//! storage: the storage engine for schedule project.
//! 
//! Basic layout:
//! (Metadata | User | Task[N])
//! 
//! We use `mmap` on posix platforms.
#ifndef __unix__
#error Only support Unix platform
#else

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "storage.h"

struct Metadata
{
    size_t used;
};

Storage::Storage(): fd(-1), mapping(MAP_FAILED) {}

Storage::Storage(const char *name): Storage()
{
    signin(name);
}

Storage::Storage(const User &user): Storage()
{
    signup(user);
}

Storage::~Storage()
{
    if(!fail()) signout();
}

bool Storage::fail() const
{
    return fd == -1 || mapping == MAP_FAILED;
}

void Storage::signup(const User &user)
{
    // create file
    char filepath[DATADIR_SIZE + USERNAME_SIZE] = DATADIR;
    strcat(filepath, user.name);
    fd = open(filepath, O_RDWR | O_CREAT | O_EXCL, 0666);
    if(fd == -1) return;

    // initialize
    Metadata meta = { sizeof(Metadata) + sizeof(User) };
    mapsize = sizeof(Metadata) + sizeof(User) + 2 * sizeof(Task);
    if(ftruncate(fd, mapsize) == -1) goto error; // reserve 2 task space
    if(write(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(write(fd, &user, sizeof(User)) == -1) goto error;

    // memory map
    mapping = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mapping == MAP_FAILED) goto error;

    used = &((Metadata *)mapping)->used;

end:
    return;
error:
    close(fd);
    fd = -1;
}

void Storage::signin(const char *name)
{
    // open file
    if(strlen(name) >= USERNAME_SIZE) return; // invalid name length
    char filepath[DATADIR_SIZE + USERNAME_SIZE] = DATADIR;
    strcat(filepath, name);
    fd = open(filepath, O_RDWR, 0666);
    if(fd == -1) return;

    // check file
    Metadata meta;
    User user;
    if(read(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(read(fd, &user, sizeof(User)) == -1) goto error;

    // file status
    struct stat filestatus;
    if(fstat(fd, &filestatus) == -1) goto error;
    mapsize = filestatus.st_size;

    // memory map
    mapping = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mapping == MAP_FAILED)
    {
        close(fd);
        fd = -1;
        return;
    }

    used = &((Metadata *)mapping)->used;

end:
    return;
error:
    close(fd);
    fd = -1;
}

void Storage::signout()
{
    munmap(mapping, mapsize);
    mapping = MAP_FAILED;
    close(fd);
    fd = -1;
}

void Storage::cancel()
{
    char filepath[DATADIR_SIZE + USERNAME_SIZE] = DATADIR;
    strcat(filepath, user().name);
    signout();
    remove(filepath);
}

User &Storage::user()
{
    return *(User *)((uint8_t *)mapping + sizeof(Metadata));
}

Task *Storage::tasks(uint64_t &len)
{
    len = (*used - sizeof(Metadata) - sizeof(User)) / sizeof(Task);
    return (Task *)((uint8_t *)mapping + sizeof(Metadata) + sizeof(User));
}

void Storage::insert_task(const Task &task)
{
    // allocate new block
    if(*used + sizeof(Task) > mapsize)
    {
        // dellocate
        munmap(mapping, mapsize);

        // resize the file
        mapsize = mapsize * 3 / 2; // soundness: mapsize > tasksize * 2
        if(ftruncate(fd, mapsize) == -1) goto error;
        printf("new block: %lu\n", mapsize);

        // memory map
        mapping = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(mapping == MAP_FAILED) goto error;

        // initialize
        used = &((Metadata *)mapping)->used;
    }

    // insert new task
    memcpy((uint8_t *)mapping + *used, &task, sizeof(Task));
    *used += sizeof(Task);
    printf("insert at %lu\n", *used);

end:
    return;
error:
    close(fd);
    fd = -1;
}

#endif