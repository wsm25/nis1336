#ifndef SCHEDULE_ACCOUNT_H
#define SCHEDULE_ACCOUNT_H
#include <cstdint>

struct Tag {
    int count; // reference of the tag
    char name[16];
};

class User {
    char name[20];
    char hashpwd[20]; // sha-1
    unsigned tagcount, taskcount; // count of tags and tasks
    // tags is bound to a user. Task should only record tag id(index).
    Tag tags[128];
public:
    /// Add a tag. If tags exists, increase its count;
    /// if tag is too long, or tag array is full, return false.
    bool add_tag(char* tag);
    /// Set password
    void set_password(char* password);
    /// Verify password
    bool verify_password(char* password);
    /// Returns tags bound to the id
    char* tag(int id){return tags[id].name;}
};

#endif // SCHEDULE_ACCOUNT_H