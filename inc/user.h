#ifndef SCHEDULE_USER_H
#define SCHEDULE_USER_H

#include <cstdint>
#include "consts.h"
// include the header "pathToLib/sha1.h" to use sha_512
// If you are using C++14 or C++17, don't forget the "gsl" folder!
#include "Hash/src/sha2_512.h"
#include <cstring>
#include <string>

typedef char Tag[TAGNAME_SIZE];
struct HashedPassword {
    char hashedPassword[HASHEDPASSED_SIZE];
    void initialize(const char *password)
    {
        // Get an instance
        Chocobo1::SHA2_512 sha2;

        // Feed data & data length to it
        sha2.addData(password, strlen(password));

        // Tell it to wrap it up
        sha2.finalize();

        // Profit! oops, not really...
        std::string str_hashedPassword = sha2.toString();

        //turn string to char
        int i = 0;
        for(;str_hashedPassword[i] != '\0';++i)
        {
            hashedPassword[i] = str_hashedPassword[i];
        }
        hashedPassword[i] = '\0';
    }
}; // sha_512


class User {
    char name[USERNAME_SIZE];
    HashedPassword hashpwd;
    bool pwdunset; // whether password is unset
    // tags is bound to a user. Task should only record tag id(index).
    Tag tags[MAX_TAGS_PER_USER];
    unsigned tagcount; // count of tags

public:
    // Remain `user` uninitialized
    User();
    /// Initialize all fields
    User(const char *username);
    /// Add a tag. If tags exists, increase its count;
    /// if tag is too long, or tag array is full, return false.
    bool add_tag(const char *tag);
    /// Set username
    bool set_username(const char *username);
    /// Set password
    bool set_password(const char *password);
    /// Verify password
    bool verify_password(const char *password);
    /// Whether password is unset
    bool password_unset() const { return pwdunset; }
    /// Return username
    const char *Name() const { return name; }
    /// Return tag name of tag with given id
    const char *tag(int id) const { return tags[id]; }
};

#endif // SCHEDULE_USER_H