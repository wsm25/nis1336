#ifndef SCHEDULE_USER_H
#define SCHEDULE_USER_H
#include <cstdint>

#include "storage.h"
#include "consts.h"

typedef char Tag[TAGNAME_SIZE];
typedef struct {} HashedPassword; // argon2

class User {
    char name[USERNAME_SIZE];
    HashedPassword hashpwd;
    bool pwdunset; // whether password is unset
    // tags is bound to a user. Task should only record tag id(index).
    Tag tags[MAX_TAGS_PER_USER];
    unsigned tagcount; // count of tags
    friend class Storage;
public:
    // Remain `user` uninitialized
    User(){} 
    /// Initialize all fields
    User(const char* username);
    /// Add a tag. If tags exists, increase its count;
    /// if tag is too long, or tag array is full, return false.
    bool add_tag(const char* tag);
    /// Set password
    void set_password(const char* password);
    /// Verify password
    bool verify_password(const char* password);
    /// Whether password is unset
    bool password_unset(){return pwdunset;}
    /// Return tag name of tag with given id
    const char* tag(int id){return tags[id];}
};

#endif // SCHEDULE_USER_H