#ifndef SHELL_H
#define SHELL_H

#include "Command.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

namespace terminal
{
    int help();
    int invalidCommand(int argc, char *argv []);
    int signup(const char *user_name, const char *password);
    int signin(const char *user_name, const char *password, Storage &using_file);

    int editname(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int editpwd(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int cancel(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int addtask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int edittask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int showtask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);
    int deltask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks);

    typedef std::unordered_map<std::string, int(*)(std::istringstream &, Storage &, Tasks &)> CMDS;
    const CMDS cmds = {{"editname", editname}, {"editpwd", editpwd}, {"cancel", cancel}, 
        {"addtask", addtask}, {"edittask", edittask}, {"showtask", showtask}, {"deltask", deltask}};
}

namespace schedule
{
    void help(std::istringstream &iss, bool &isstopped);
    void quit(std::istringstream &iss, bool &isstopped);
    void signin(std::istringstream &iss, bool &isstopped);
    void signup(std::istringstream &iss, bool &isstopped);

    typedef std::unordered_map<std::string, void(*)(std::istringstream &, bool &)> CMDS;
    const CMDS cmds = {{"help", help}, {"quit", quit}, 
        {"signin", signin}, {"signup", signup}};
    int shell();
}

namespace user
{
    void help(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void signout(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void editname(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void editpwd(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void cancel(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void addtask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void edittask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void showtask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);
    void deltask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks, pthread_t remind_thread);

    typedef std::unordered_map<std::string, void(*)(std::istringstream &, bool &, Storage &, Tasks &, pthread_t)> CMDS;
    const CMDS cmds = {{"help", help}, 
        {"signout", signout}, {"editname", editname}, {"editpwd", editpwd}, {"cancel", cancel}, 
        {"addtask", addtask}, {"edittask", edittask}, {"showtask", showtask}, {"deltask", deltask}};
    int shell(Storage &using_file);
}

#endif