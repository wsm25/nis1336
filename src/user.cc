#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

#include "user.h"

// Remain `user` uninitialized
User::User(): name(), pwdunset(true), tagcount(0), tags() {}

/// Initialize all fields
User::User(const char *username): User()
{
    // invalid name length
    if(strlen(username) >= USERNAME_SIZE)
    {
        std::cerr << "User: Your name is too long" << std::endl;
        return;
    }
    strcpy(name, username);
    pwdunset = true;
    tagcount = 0;
    ///default:
    hashpwd.initialize("hello");
}

/// Add a tag. If tags exists, increase its count;
/// if tag is too long, or tag array is full, return false.
///tags[] is 0-based
bool User::add_tag(const char *tag)
{
    int taglen = strlen(tag);
    if(taglen > TAGNAME_SIZE || (tagcount + 1) > MAX_TAGS_PER_USER)
    {
        std::cerr << "Tag: Length error" << std::endl;
        return false;
    }

    strcpy(tags[tagcount], tag);
    tagcount++;
    return true;
}

/// Set username
bool User::set_username(const char *username)
{
    if(strlen(username) >= USERNAME_SIZE)
    {
        std::cerr << "User: Your name is too long" << std::endl;
        return false;
    }
    strcpy(name, username);
    return true;
}

/// Set password 
bool User::set_password(const char *password)
{
    //invalid password length
    if(strlen(password) >= PASSWORD_SIZE)
    {
        std::cerr << "User: Your password is too long" << std::endl;
        return false;
    }
    hashpwd.initialize(password);
    pwdunset = false;
    return true;
}

/// Verify password 
bool User::verify_password(const char *password)
{
    HashedPassword hashed_input_password;
    hashed_input_password.initialize(password);
    if(strcmp(hashed_input_password.hashedPassword, hashpwd.hashedPassword) == 0)
        return true;
    else return false;
}