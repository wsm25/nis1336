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
#include <string>

#include "storage.h"
#include "user.h"
#include "task.h"

struct Metadata{
    int used;
};

Storage::Storage(const char* name){
    if(strlen(name)>=20) return; // invalid name length
    std::string path("data/");
    path+=name;
    const char* filepath=path.c_str();
    fd=open(filepath, O_RDWR, 0666);
    User user;
    Metadata meta;
    if(fd==-1){ // open failed, try create
        fd=open(filepath, O_RDWR|O_CREAT, 0666);
        if(fd==-1) goto error;
        // TODO: init password
        meta.used=sizeof(Metadata)+sizeof(User);
        new(&user) User(name);
        write(fd, &meta, sizeof(Metadata));
        write(fd, &user, sizeof(User));
        ftruncate(fd, meta.used+2*sizeof(Task)); // reserve 2 task space
    } else {
        if(read(fd, &meta, sizeof(Metadata))==-1) goto error;
        if(read(fd, &user, sizeof(User))==-1) goto error;
    }
    used=meta.used;
    // file status
    struct stat sb;
    if(fstat(fd, &sb)!=0) goto error;
    mapsize = sb.st_size;
    // mmap
    if(mapsize<=0) goto error;
    mapping = (char*)mmap(
        0, mapsize, 
        PROT_WRITE|PROT_READ,
        MAP_SHARED,
        fd, 0);
    if(mapping==(void*)-1) goto error;
end:
    return;
error:
    fd=-1;
    mapping=(char*)-1;
    goto end;
}

Storage::~Storage(){
    munmap(mapping, mapsize);
    close(fd);
    fd=-1;
}

void Storage::cancel(){
    std::string path("db/");
    path+=user().name;
    munmap(mapping, mapsize);
    close(fd);
    fd=-1;
    remove(path.c_str());
}

User& Storage::user(){
    return *(User*)(mapping+sizeof(Metadata));
}

Task* Storage::tasks(int& len){
    len = (used-sizeof(Metadata)+sizeof(User))/sizeof(Task);
    return (Task*)(mapping+sizeof(Metadata)+sizeof(User));
}

void Storage::insert_task(const Task& task){
    if(used+(ssize_t)sizeof(Task)>mapsize){ // allocate new block
        mapsize=mapsize*3/2; // soundness: mapsize>tasksize*2
        ftruncate(fd, mapsize);
    }
    memcpy(mapping+used, &task, sizeof(Task));
    used+=sizeof(Task);
}

#endif