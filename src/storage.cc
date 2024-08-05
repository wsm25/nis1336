//! storage: the storage engine for schedule project.
//! 
//! Basic layout:
//! (Metadata | User | Task[N])
//! 
//! We use `mmap` on posix platforms, and `MapViewOfFile` on windows
#if defined(_WIN32)
#include <windows.h>
#elif defined(__unix__)
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#else
#error Current platform is not supported
#endif

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "storage.h"
#include <mutex>

#if defined(_WIN32)
#include <winerror.h>
void PrintError(const char *ErrMsg)
{
    LPVOID lpvMessageBuffer;

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), 0, (LPSTR)&lpvMessageBuffer, 0, NULL);

    std::cerr << ErrMsg << ": " << (char *)lpvMessageBuffer;

    LocalFree(lpvMessageBuffer);
}
#elif defined(__unix__)
#include <errno.h>
#endif

std::mutex mutex;

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
    strcpy(filepath, DATAPATH);
    strcat(filepath, filename);
}

void Storage::reserve(size_t capacity)
{
    mutex.lock();
#if defined(_WIN32)
    HANDLE hFileMappingObject;
#endif

    // unmap
    if(!fail())
#if defined(_WIN32)
        if(!UnmapViewOfFile(mapping)) goto error;
#elif defined(__unix__)
        if(munmap(mapping, mapsize) == -1) goto error;
#endif

    // resize the file
    if(mapsize < capacity)
    {
        mapsize = capacity;
#ifdef __unix__
        if(ftruncate(fd, mapsize) == -1) goto error;
#endif
        //printf("new block: %lu\n", mapsize);
    }

    // memory map
#if defined(_WIN32)
    hFileMappingObject = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, mapsize >> 32, mapsize & 0xffffffff, NULL);
    if(hFileMappingObject == NULL) goto error;
    mapping = MapViewOfFile(hFileMappingObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(!CloseHandle(hFileMappingObject) || mapping == NULL) goto error;
#elif defined(__unix__)
    mapping = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mapping == MAP_FAILED) goto error;
#endif

    // initialize
    used = &((Metadata *)mapping)->used;

end:
    mutex.unlock();
    return;
error:
    mapsize = 0;
#if defined(_WIN32)
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    PrintError("reserve");
#elif defined(__unix__)
    close(fd);
    fd = -1;
    perror("reserve");
#endif
    goto end;
}

#if defined(_WIN32)
Storage::Storage(): hFile(INVALID_HANDLE_VALUE), mapping(NULL), mapsize(0) {}
#elif defined(__unix__)
Storage::Storage() : fd(-1), mapping(MAP_FAILED), mapsize(0) {}
#endif

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
#if defined(_WIN32)
    return hFile == INVALID_HANDLE_VALUE || mapping == NULL;
#elif defined(__unix__)
    return fd == -1 || mapping == MAP_FAILED;
#endif
}

void Storage::signup(const User &user)
{
    // sign out
    if(!fail()) signout();

    // create file
    char filepath[FILEPATH_SIZE];
    get_filepath(user.Name(), filepath);
#if defined(_WIN32)
    hFile = CreateFileA(filepath,
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        if(GetLastError() == ERROR_FILE_EXISTS) std::cerr << RED << "signup: Username exists" << RESET << std::endl;
        else PrintError("signup");
        return;
    }
#elif defined(__unix__)
    fd = open(filepath, O_RDWR | O_CREAT | O_EXCL, 0666);
    if(fd == -1)
    {
        if(errno == EEXIST) std::cerr << RED << "signup: Username exists" << RESET << std::endl;
        else perror("signup");
        return;
    }
#endif

    // initialize
    Metadata meta = {sizeof(Metadata) + sizeof(User)};
#if defined(_WIN32)
    DWORD numberOfCharsToRead = 0;
    if(!WriteFile(hFile, &meta, sizeof(Metadata), &numberOfCharsToRead, NULL)) goto error;
    if(numberOfCharsToRead != sizeof(Metadata))
    {
        std::cerr << RED << "signup: failed to write meta" << RESET << std::endl;
        goto error;
    }
    if(!WriteFile(hFile, &user, sizeof(User), &numberOfCharsToRead, NULL)) goto error;
    if(numberOfCharsToRead != sizeof(User))
    {
        std::cerr << RED << "signup: failed to write user" << RESET << std::endl;
        goto error;
    }
#elif defined(__unix__)
    if(write(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(write(fd, &user, sizeof(User)) == -1) goto error;
#endif
    reserve(meta.used + 2 * sizeof(Task)); // reserve 2 task space

end:
    std::cout << "New user: " << user.Name() << std::endl;
    return;
error:
#if defined(_WIN32)
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    PrintError("signup");
#elif defined(__unix__)
    close(fd);
    fd = -1;
    perror("sinup");
#endif
    goto end;
}

void Storage::signin(const char *name)
{
    // sign out
    if(!fail()) signout();

    // open file
    if(strlen(name) >= USERNAME_SIZE) // invalid name length
    {
        std::cerr << RED << "User: Your name is too long" << RESET << std::endl;
        return;
    }
    char filepath[FILEPATH_SIZE];
    get_filepath(name, filepath);
#if defined(_WIN32)
    hFile = CreateFileA(filepath,
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        if(GetLastError() == ERROR_FILE_NOT_FOUND) std::cerr << RED << "signin: No such user" << RESET << std::endl;
        else PrintError("signin");
        return;
    }
#elif defined(__unix__)
    fd = open(filepath, O_RDWR, 0666);
    if(fd == -1)
    {
        if(errno == ENOENT) std::cerr << RED << "signin: No such user" << RESET << std::endl;
        else perror("signin");
        return;
    }
#endif

    // check file
    Metadata meta;
    User user;
#if defined(_WIN32)
    DWORD numberOfCharsToRead = 0;
    if(!ReadFile(hFile, &meta, sizeof(Metadata), &numberOfCharsToRead, NULL)) goto error;
    if(numberOfCharsToRead != sizeof(Metadata))
    {
        std::cerr << RED << "signin: failed to read meta" << RESET << std::endl;
        goto error;
    }
    if(!ReadFile(hFile, &user, sizeof(User), &numberOfCharsToRead, NULL)) goto error;
    if(numberOfCharsToRead != sizeof(User))
    {
        std::cerr << RED << "signin: failed to read user" << RESET << std::endl;
        goto error;
    }
    if(strcmp(name, user.Name())) { SetLastError(ERROR_DATA_CHECKSUM_ERROR); goto error; }
#elif defined(__unix__)
    if(read(fd, &meta, sizeof(Metadata)) == -1) goto error;
    if(read(fd, &user, sizeof(User)) == -1) goto error;
    if(strcmp(name, user.Name())) { errno = ENODATA; goto error; }
#endif

    // file status
#if defined(_WIN32)
    DWORD fileSizeHigh;
    mapsize = GetFileSize(hFile, &fileSizeHigh);
    if(mapsize == INVALID_FILE_SIZE) { mapsize = 0; goto error; }
    mapsize += (size_t)fileSizeHigh << 32;
#elif defined(__unix__)
    struct stat filestatus;
    if(fstat(fd, &filestatus) == -1) goto error;
    mapsize = filestatus.st_size;
#endif

    //initialize
    reserve(mapsize);

end:
    return;
error:
#if defined(_WIN32)
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    PrintError("signin");
#elif defined(__unix__)
    close(fd);
    fd = -1;
    perror("signin");
#endif
    goto end;
}

void Storage::signout()
{
#if defined(_WIN32)
    if(!UnmapViewOfFile(mapping)) PrintError("signout");
    mapping = NULL;
    mapsize = 0;
    if(!CloseHandle(hFile)) PrintError("signout");
    hFile = INVALID_HANDLE_VALUE;
#elif defined(__unix__)
    if(munmap(mapping, mapsize) == -1) perror("signout");
    mapping = MAP_FAILED;
    mapsize = 0;
    if(close(fd) == -1) perror("signout");
    fd = -1;
#endif
}

void Storage::cancel()
{
    char filepath[FILEPATH_SIZE];
    get_filepath(user().Name(), filepath);
    signout();
    if(remove(filepath) == -1) perror("cancel");
}

bool Storage::edit_name(const char *name)
{
    if(!User().set_username(name)) return false;
    char newpath[FILEPATH_SIZE];
    get_filepath(name, newpath);
    if(std::ifstream{newpath})
    {
        std::cerr << RED << "editname: Username exists" << RESET << std::endl;
        return false;
    }

#if defined(_WIN32)
    if(!CloseHandle(hFile))
    {
        PrintError("signout");
        return false;
    }
#endif

    char oldpath[FILEPATH_SIZE];
    get_filepath(user().Name(), oldpath);
    if(rename(oldpath, newpath) == -1)
    {
        perror("editname");
        return false;
    }
    user().set_username(name);

#if defined(_WIN32)
    hFile = CreateFileA(newpath,
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        PrintError("editname");
        return false;
    }
#endif

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
    //printf("insert at %lu\n", *used);
    uint64_t len;
    tasks(len);
    std::cout << "addtask: " << std::left << std::setw(6) << len - 1 << " ";
    task.showtask();
}
