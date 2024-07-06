#ifndef SCHEDULE_USER_H
#define SCHEDULE_USER_H
#include <cstdint>

#include "storage.h"
#include "consts.h"
// include the header "pathToLib/sha1.h" to use sha_512
// If you are using C++14 or C++17, don't forget the "gsl" folder!
#include "C:\Users\Lipeiying\Desktop\Hash-master\Hash-master\src/sha1.h"
#include <iostream>
#include<cstring>
#include<string>

typedef char Tag[TAGNAME_SIZE];
typedef struct {
    string hashedPassword;
    bool initialize(const char* password)
    {
        // Get an instance
        Chocobo1::SHA1 sha1;

        // Feed data & data length to it
        sha1.addData(password,strlen(password));

         // Tell it to wrap it up
        sha1.finalize();

        // Profit! oops, not really...
        std::string hashedPassword = sha1.toString();
    }
} HashedPassword; // sha_512


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