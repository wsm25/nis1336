#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

#include "storage.h"
#include "user.h"
#include "task.h"


using namespace std;

/// Initialize all fields
User :: User(const char* username)
{
    int i = 0;
    for (;username[i] != '\0' ;++i)
        name[i] = username[i];
    name[i] = '\0';
    pwdunset = true;
    tagcount = 0;
    ///default:
    hashpwd.initialize("hello");
}

/// Add a tag. If tags exists, increase its count;
/// if tag is too long, or tag array is full, return false.
///tags[] is 0-based
bool User :: add_tag(const char* tag)
{
    int taglen = strlen(tag);
    if (taglen > TAGNAME_SIZE || (tagcount+1) > MAX_TAGS_PER_USER) 
    {
        cout << "OVERFLOW" << endl;
        return false;
    }
    
    strcpy(tags[tagcount],tag);
    tagcount ++;
    return true;
}

/// Set password 
void User :: set_password(const char* password)
{
    hashpwd.initialize(password);
    pwdunset = false;
}

/// Verify password 
bool User :: verify_password(const char* password)
{
    HashedPassword hashed_input_password;
    hashed_input_password.initialize(password);
    if(strcmp(hashed_input_password.hashedPassword,hashpwd.hashedPassword) == 0)
        return true;
    else return false;
}