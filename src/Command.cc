#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <iomanip>

// auxiliary function

// error message
int invalidCommand(std::istringstream &iss)
{
    std::cerr << RED << "Invalid command: " << iss.str() << "\ntry help" << RESET << std::endl;
    return 1;
}

// parsing
// parse username with hint
bool parseUsernameHint(std::istringstream &iss, std::string &username)
{
    if(iss.eof())
    {
        std::cout << "Username: " << std::flush;
        std::getline(std::cin, username);
        if(username.find(' ') != username.npos) goto error;
        else return true;
    }
    else
    {
        iss >> username;
        if(iss.fail()) goto error;
        else return true;
    }

error:
    std::cerr << RED << "User: Invalid name" << RESET << std::endl;
    return false;
}

// getpass
#if defined(_WIN32)
#include <windows.h>

std::string getpass(const char *prompt)
{
    std::string password;

    std::cout << prompt << std::flush;

    HANDLE consoleInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(consoleInput, &mode);
    SetConsoleMode(consoleInput, mode & ~ENABLE_ECHO_INPUT);

    std::getline(std::cin, password);
    std::cout << std::endl;

    SetConsoleMode(consoleInput, mode);

    return password;
}

#elif defined(__unix__)
#include <unistd.h>
// char *getpass(const char *__prompt);
#else
#error Current platform is not supported
#endif

// parse password with hint
void parsePwdHint(std::istringstream &iss, std::string &password)
{
    if(iss.eof())
        password = getpass("Password: ");
    else
        iss >> password;
}

// convert string to time_t
bool convertToTimeT(const std::string &dateTimeStr, time_t &t)
{
    std::istringstream iss(dateTimeStr);
    std::tm tmTime = *localtime(&t); // 获取默认时间（当前时间/开始时间）的tm结构

    if(dateTimeStr.find('/') != std::string::npos) // 包含日期
        iss >> std::get_time(&tmTime, "%Y/%m/%d-%T");
    else // 仅时间，使用当前日期
        iss >> std::get_time(&tmTime, "%T");

    if(!iss.eof() || iss.fail()) // 数据未完全读取或读取过程中出错
    {
        std::cerr << RED << "Time: Failed to parse time." << RESET << std::endl;
        return false;
    }
    else
    {
        t = mktime(&tmTime);
        return true;
    }
}

//parse Task
bool parseTask(std::istringstream &iss, Task &t)
{
    //default
    if(iss.eof()) return true;
    std::string word;
    // 用来存储分割后的单词
    std::vector<std::string> words;

    //防止先输入-e或者-r,再输入-b将其覆盖
    bool end_set = false;
    bool remind_set = false;
    // 循环读取每个单词
    while(iss >> word)
        words.push_back(word);

    int i = 0;
    auto it = words.begin() + i;
    if(it == words.end())
    {
        invalidCommand(iss);
        return false;
    }
    while(it != words.end())
    {
        //检查指令后是否有参数
        if(it == words.end())
        {
            std::cerr << RED << "Parameter missing" << RESET << std::endl;
            invalidCommand(iss);
            return false;
        }
        //此时it为输入参数

        if(*it == "-n")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            if(std::strlen(Con) >= TASKNAME_SIZE)
            {
                std::cerr << RED << "Task: Name is too long" << RESET << std::endl;
                return false;
            }
            t.name = Con;
        }

        else if(*it == "-c")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            if(std::strlen(Con) >= TASKCONTENT_SIZE)
            {
                std::cerr << RED << "Task: Content is too long" << RESET << std::endl;
                return false;
            }
            strcpy(t.content, Con);
        }

        else if(*it == "-b")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            if(!convertToTimeT(*it, t.begin)) return false;
            if(!end_set)
                t.end = t.begin;
            if(!remind_set)
                t.remind.t = t.begin;
        }

        else if(*it == "-e")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            if(!convertToTimeT(*it, t.end)) return false;
            end_set = true;
        }

        else if(*it == "-r")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            if(!convertToTimeT(*it, t.remind.t)) return false;
            remind_set = true;
        }

        else if(*it == "-p")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            if(!strcmp(Con, "low"))
                t.priority = Task::Low;
            else if(!strcmp(Con, "mid"))
                t.priority = Task::Mid;
            else if(!strcmp(Con, "high"))
                t.priority = Task::High;
            else return false;
        }

        else if(*it == "-t")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            if(strlen(Con) >= TAGNAME_SIZE)
            {
                std::cerr << RED << "Tag is too long" << RESET << std::endl;
                return false;
            }
            int j = 0;
            for(; Con[j] != '\0'; ++j)
                t.tags[j] = Con[j];
            t.tags[j] = '\0';
        }

        else if(*it == "-s")
        {
            if(++it == words.end())
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            if(!strcmp(Con, "unfin"))
                t.status = Task::Unfinished;
            else if(!strcmp(Con, "fin"))
                t.status = Task::Finished;
            else if(!strcmp(Con, "abort"))
                t.status = Task::Abort;
            else return false;
        }

        else
        {
            invalidCommand(iss);
            return false;
        }

        if(it == words.end()) break;
        it++;
        i++;
    }//while结束
    if(t.begin > t.end)
    {
        std::cerr << RED << "Task: begin time is later than end time" << RESET << std::endl;
        return false;
    }
    return true;
}

//parse taskID
bool parseTaskID(std::istringstream &iss, Tasks &using_tasks, size_t &id)
{
    iss >> id;
    if(iss.fail())
    {
        invalidCommand(iss);
        return false;
    }
    else if(id >= using_tasks.size() || using_tasks[id].status == Task::Abort)
    {
        std::cerr << RED << "Tasks: No such task" << RESET << std::endl;
        return false;
    }
    else
        return true;
}

///user-related command
int signup(std::istringstream &iss, Storage &using_file)
{
    User user;
    std::string username, password;

    //<username>
    //allow typing with hints
    if(!parseUsernameHint(iss, username)) return 1;
    if(!user.set_username(username.c_str())) return 1;

    //<password>
    parsePwdHint(iss, password);
    if(!user.set_password(password.c_str())) return 1;

    if(iss.eof())
    {
        using_file.signup(user);
        return using_file.fail();
    }
    else
        return invalidCommand(iss);
}

int signin(std::istringstream &iss, Storage &using_file)
{
    std::string username, password;

    //<username>
    if(!parseUsernameHint(iss, username)) return 1;
    using_file.signin(username.c_str());
    if(using_file.fail()) return 1;

    //<password>
    parsePwdHint(iss, password);
    if(!using_file.user().verify_password(password.c_str()))
    {
        using_file.signout();
        std::cerr << RED << "signin: Wrong password" << RESET << std::endl;
        return 1;
    }

    if(iss.eof()) return 0;
    else return invalidCommand(iss);
}

int editname(std::istringstream &iss, Storage &using_file)
{
    std::string username;
    if(!parseUsernameHint(iss, username)) return 1;
    if(iss.eof())
        return !using_file.edit_name(username.c_str());
    else
        return invalidCommand(iss);
}

int editpwd(std::istringstream &iss, Storage &using_file)
{
    std::string password;
    parsePwdHint(iss, password);
    if(iss.eof())
        return !using_file.user().set_password(password.c_str());
    else
        return invalidCommand(iss);
}

int cancel(std::istringstream &iss, Storage &using_file)
{
    if(iss.eof())
    {
        using_file.cancel();
        return 0;
    }
    else
        return invalidCommand(iss);
}

///task-related command
int addtask(std::istringstream &iss, Tasks &using_tasks)
{
    Task t;
    if(!parseTask(iss, t)) return 1;
    if(iss.eof())
        return !using_tasks.insert(t);
    else
        return invalidCommand(iss);
}

int deltask(std::istringstream &iss, Tasks &using_tasks)
{
    size_t id;
    if(!parseTaskID(iss, using_tasks, id)) return 1;
    if(iss.eof())
    {
        using_tasks[id].status = Task::Abort;
        return 0;
    }
    else
        return invalidCommand(iss);
}

int edittask(std::istringstream &iss, Tasks &using_tasks)
{
    size_t id;
    if(!parseTaskID(iss, using_tasks, id)) return 1;
    if(!parseTask(iss, using_tasks[id])) return 1;
    return 0;
}

int showtask_aux(const std::vector<size_t> &v, Tasks &using_tasks)
{
    if(v.empty())
    {
        std::cout << "showtask: No tasks" << std::endl;
        return 1;
    }
    std::cout << std::left << std::setw(TASKID_SIZE) << "taskID" << " ";
    std::cout << std::left << std::setw(TASKNAME_SIZE) << "name";
    std::cout << std::left << std::setw(TIMENAME_SIZE) << "begin time";
    std::cout << std::left << std::setw(TIMENAME_SIZE) << "end time";
    std::cout << std::left << std::setw(TIMENAME_SIZE) << "remind time";
    std::cout << std::left << std::setw(PRINAME_SIZE) << "pri";
    std::cout << std::left << std::setw(STATNAME_SIZE) << "stat";
    std::cout << std::left << std::setw(TAGNAME_SIZE) << "tag" << std::endl;
    std::cout << "\t" << "content" << std::endl;

    for(auto i : v)
    {
        std::cout << std::left << std::setw(TASKID_SIZE) << i << " ";
        using_tasks[i].showtask();
    }
    return 0;
}

int show_high_pri(Tasks &using_tasks)
{
    std::cout << "Tasks with high priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::High);
    return showtask_aux(v, using_tasks);
}

int show_mid_pri(Tasks &using_tasks)
{
    std::cout << "Tasks with middle priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::Mid);
    return showtask_aux(v, using_tasks);
}

int show_low_pri(Tasks &using_tasks)
{
    std::cout << "Tasks with low priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::Low);
    return showtask_aux(v, using_tasks);
}

int show_by_tag(Tasks &using_tasks, std::string &word)
{
    std::cout << "Tasks with tag: " << word << std::endl;
    auto v = using_tasks.select([&](const Task &x) -> bool
        {
            return !strcmp(x.tags, word.c_str());
        });
    return showtask_aux(v, using_tasks);
}

bool show_by_day(int days, const Task &using_task)
{
    time_t rawTime = time(0);
    struct tm now = *localtime(&rawTime);
    struct tm task_begin = *localtime(&using_task.begin);
    // Set hours, minutes, seconds to 0 to compare only dates
    now.tm_hour = 0;
    now.tm_min = 0;
    now.tm_sec = 0;
    now.tm_mday = now.tm_mday + days;
    task_begin.tm_hour = 0;
    task_begin.tm_min = 0;
    task_begin.tm_sec = 0;
    time_t now_date_only = mktime(&now);
    time_t task_date_only = mktime(&task_begin);
    return now_date_only >= task_date_only;
}

int show_day(Tasks &using_tasks, int days)
{
    std::cout << "Tasks in " << days << " days: " << std::endl;
    auto v = using_tasks.select([&days](const Task &x) -> bool
        {
            return show_by_day(days, x);
        });

    return showtask_aux(v, using_tasks);
}

int showtask(std::istringstream &iss, Tasks &using_tasks)
{
    //default
    if(iss.eof())
    {
        std::cout << "Tasks by begin time:" << std::endl;
        auto v = using_tasks.sort(&Task::begin);
        return showtask_aux(v, using_tasks);
    }

    std::string word;
    iss >> word;
    if(iss.fail() || !iss.eof())
    {
        if(word == "-p")
        {
            iss >> word;
            if(iss.fail() || !iss.eof())
                return invalidCommand(iss);
            if(strcmp(word.c_str(), "high") == 0)
                return show_high_pri(using_tasks);
            else if(strcmp(word.c_str(), "mid") == 0)
                return show_mid_pri(using_tasks);
            else if(strcmp(word.c_str(), "low") == 0)
                return show_low_pri(using_tasks);
            else
            {
                std::cerr << RED << "No such priority" << RESET << std::endl;
                return 1;
            }
        }
        else if(word == "-t")
        {
            iss >> word;
            if(iss.fail() || !iss.eof())
                return invalidCommand(iss);
            return show_by_tag(using_tasks, word);
        }
        else if(word == "-d")
        {
            int days = 0;
            iss >> days;
            if(iss.fail() || !iss.eof())
                return invalidCommand(iss);
            return show_day(using_tasks, days);
        }
        else return invalidCommand(iss);
    }

    //iss.eof();
    if(word == "-r")
    {
        std::cout << "Tasks by remind time:" << std::endl;
        auto v = using_tasks.sort(&Task::remind);
        return showtask_aux(v, using_tasks);
    }

    else if(word == "-b")
    {
        std::cout << "Tasks by begin time:" << std::endl;
        auto v = using_tasks.sort(&Task::begin);
        return showtask_aux(v, using_tasks);
    }

    else if(word == "-e")
    {
        std::cout << "Tasks by end time:" << std::endl;
        auto v = using_tasks.sort(&Task::end);
        return showtask_aux(v, using_tasks);
    }

    else if(word == "-p")
    {
        std::cout << "Tasks by priority:" << std::endl;
        auto v = using_tasks.sort(&Task::begin);
        return showtask_aux(v, using_tasks);
    }

    else if(word == "-d")
        return show_day(using_tasks, 0);

    else
        return invalidCommand(iss);
    return 0;
}
