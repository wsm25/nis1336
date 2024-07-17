#ifndef SHELL_H
#define SHELL_H

#include "Command.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

class terminal
{
    ///command
public:
    static int help();
    static int invalidCommand(int argc, char *argv []);
    static int signup(const char *user_name, const char *password);
    static int signin(const char *user_name, const char *password);
private:
    static int editname(); static int editpwd(); static int cancel();
    static int addtask(); static int edittask(); static int showtask(); static int deltask();

    ///member
    static std::istringstream iss;
    static Storage using_file;
    static Tasks using_tasks;

    typedef std::unordered_map<std::string, int(*)()> CMDS;
    static const CMDS cmds;

public:
    static int shell(int argc, char *argv[]);
};

class schedule
{
private:
    ///command
    static void help(); static void quit();
    static void signin(); static void signup();

    ///member
    static std::istringstream iss;
    static bool isstopped;
    typedef std::unordered_map<std::string, void(*)()> CMDS;
    static const CMDS cmds;

public:
    static int shell();

    friend class user;
};

class user
{
private:
    ///commands
    static void help(); static void quit(); static void signout(); 
    static void editname(); static void editpwd(); static void cancel();
    static void addtask(); static void edittask(); static void showtask(); static void deltask();
    //thread-related function
    static void *remind(void *arg);
    
    ///member
    static std::istringstream iss;
    static bool isstopped;
    static Storage using_file;
    static Tasks using_tasks;
    static pthread_t remind_thread;
    typedef std::unordered_map<std::string, void(*)()> CMDS;
    static const CMDS cmds;

public:
    static int shell();

    friend class schedule;
};

#endif