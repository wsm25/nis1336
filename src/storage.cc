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
#include <errno.h>
#include <iostream>
#include "storage.h"
#include <pthread.h>

pthread_mutex_t lock;

struct Metadata
{
    size_t used;
};

void Storage::hex_encode(const char *raw, char *encoded)
{
    for(const char *it = raw; *it; ++it, encoded += 2)
        sprintf(encoded, "%02hhx", *it);
}

void Storage::get_filepath(const char *username, char *filepath)
{
    char filename[2 * USERNAME_SIZE - 1];
    hex_encode(username, filename);
    strcpy(filepath, DATADIR);
    strcat(filepath, filename);
}

void Storage::reserve(size_t capacity)
{
    pthread_mutex_lock(&lock);
    // unmap
    if(!fail()) munmap(mapping, mapsize);

    // resize the file
    if(mapsize < capacity)
    {
        mapsize = capacity;
        if(ftruncate(fd, mapsize) == -1) goto error;
        printf("new block: %lu\n", mapsize);
    }

    // memory map
    mapping = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mapping == MAP_FAILED) goto error;

    // initialize
    used = &((Metadata *)mapping)->used;

end:
    pthread_mutex_unlock(&lock);
    return;
error:
    mapsize = 0;
    close(fd);
    fd = -1;
    perror("reserve");
}

Storage::Storage(): fd(-1), mapping(MAP_FAILED), mapsize(0) {}

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
    // sign out
    if(!fail()) signout();

    // create file
    char filepath[FILEPATH_SIZE];
    get_filepath(user.Name(), filepath);
    fd = open(filepath, O_RDWR | O_CREAT | O_EXCL, 0666);
    if(fd == -1)
    {
        if(errno == EEXIST) std::cerr << "signup: Username exists" << std::endl;
        else perror("signup");
        return;
    }

    // initialize
    Metadata meta = { sizeof(Metadata) + sizeof(User) };
    if(write(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(write(fd, &user, sizeof(User)) == -1) goto error;
    reserve(meta.used + 2 * sizeof(Task)); // reserve 2 task space

end:
    return;
error:
    close(fd);
    fd = -1;
    perror("signup");
}

void Storage::signin(const char *name)
{
    // sign out
    if(!fail()) signout();

    // open file
    if(strlen(name) >= USERNAME_SIZE) // invalid name length
    {
        std::cerr << "User: Your name is too long" << std::endl;
        return;
    }
    char filepath[FILEPATH_SIZE];
    get_filepath(name, filepath);
    fd = open(filepath, O_RDWR, 0666);
    if(fd == -1)
    {
        if(errno == ENOENT) std::cerr << "signin: No such user" << std::endl;
        else perror("signin");
        return;
    }

    // check file
    Metadata meta;
    User user;
    if(read(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(read(fd, &user, sizeof(User)) == -1) goto error;
    if(strcmp(name, user.Name())) { errno = ENODATA; goto error; }

    // file status
    struct stat filestatus;
    if(fstat(fd, &filestatus) == -1) goto error;
    mapsize = filestatus.st_size;
    
    //initialize
    reserve(mapsize);

end:
    return;
error:
    close(fd);
    fd = -1;
    perror("signin");
}

void Storage::signout()
{
    munmap(mapping, mapsize);
    mapping = MAP_FAILED;
    mapsize = 0;
    close(fd);
    fd = -1;
}

void Storage::cancel()
{
    char filepath[FILEPATH_SIZE];
    get_filepath(user().Name(), filepath);
    signout();
    remove(filepath);
}

bool Storage::edit_name(const char *name)
{
    char tmp[USERNAME_SIZE];
    strcpy(tmp, user().Name());
    char oldpath[FILEPATH_SIZE];
    get_filepath(user().Name(), oldpath);

    if(!user().set_username(name)) return false;
    char newpath[FILEPATH_SIZE];
    get_filepath(user().Name(), newpath);

    if(access(newpath, F_OK) == 0)
    {
        user().set_username(tmp);
        std::cerr << "editname: Username exists" << std::endl;
        return false;
    }

    if(rename(oldpath, newpath) == -1)
    {
        user().set_username(tmp);
        perror("editname");
        return false;
    }
    return true;
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
        reserve(mapsize * 3 / 2); // soundness: mapsize > tasksize * 2
        if(fail()) return;
    }

    // insert new task
    memcpy((uint8_t *)mapping + *used, &task, sizeof(Task));
    *used += sizeof(Task);
    printf("insert at %lu\n", *used);
}

#endif