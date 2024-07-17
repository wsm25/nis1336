#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <sstream>
#include "tasks.h"

//about error: return 0 means no error and return 1 means error
int invalidCommand(std::istringstream &iss);

///user-related command
int signup(std::istringstream &iss, Storage &using_file);
int signin(std::istringstream &iss, Storage &using_file);
int editname(std::istringstream &iss, Storage &using_file);
int editpwd(std::istringstream &iss, Storage &using_file);
int cancel(std::istringstream &iss, Storage &using_file);

///task-related command
int addtask(std::istringstream &iss, Tasks &using_tasks);
int deltask(std::istringstream &iss, Tasks &using_tasks);
int edittask(std::istringstream &iss, Tasks &using_tasks);
int showtask(std::istringstream &iss, Tasks &using_tasks);

#endif