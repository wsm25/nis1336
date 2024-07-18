#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <iomanip>
#include <chrono>


// auxiliary function

// error message
int invalidCommand(std::istringstream &iss)
{
    std::cerr << "Invalid command: " << iss.str() << "\ntry help" << std::endl;
    return 1;
}

// parsing
// parse username with hint
bool parseUsernameHint(std::istringstream &iss, std::string &username)
{
    if(iss.eof())
    {
        std::cout << "Username: ";
        getline(std::cin, username);
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
    std::cerr << "User: Invalid name" << std::endl;
    return false;
}

// parse password with hint
void parsePwdHint(std::istringstream &iss, std::string &password)
{
    if(iss.eof())
        password = std::string(getpass("Password: "));
    else
        iss >> password;
}

// parse string as struct tm
bool parseDateTime(const std::string &dateTimeStr, tm &tmTime,bool &time_correct)
{
    std::istringstream iss(dateTimeStr);
    int year, month, day;
    int hour, minute, second;

    if( iss >> year && iss.get() == '/' &&
        iss >> month && iss.get() == '/' &&
        iss >> day && iss.get() == '-' &&
        iss >> hour && iss.get() == ':' &&
        iss >> minute && iss.get() == ':' &&
        iss >> second)
    {
        // 检查是否所有数据都被正确读取
        if (!iss.eof() || 
            year < 0 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            // 数据未完全读取或读取过程中出错
            std::cerr << "Time: Invalid time input" << std::endl;
            return false;
        }
        tmTime.tm_year = year - 1900;
        tmTime.tm_mon = month - 1;
        tmTime.tm_mday = day;
        tmTime.tm_hour = hour;
        tmTime.tm_min = minute;
        tmTime.tm_sec = second;
        time_correct = true;
    }
    else
    {
        std::cerr << "Time: Failed to parse time." << std::endl;
    }

    return true;
}

// convert string to time_t
time_t convertToTimeT(const std::string &input,bool &time_correct)
{
    std::tm currentTime = {};
    if(input.find('/') != std::string::npos) // 包含日期
    {
        parseDateTime(input, currentTime,time_correct);
    }
    else // 仅时间，使用当前日期
    {
        // 将time_t类型的时间转换为tm结构，考虑本地时区
        time_t rawTime = time(0);
        currentTime = *localtime(&rawTime); // 获取当前时间的tm结构
        int hour, minute, second;

        std::istringstream iss(input);
        if( iss >> hour && iss.get() == ':' &&
            iss >> minute && iss.get() == ':' &&
            iss >> second)
        {
            if (!iss.eof() || 
                hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
            {
                // 数据未完全读取或读取过程中出错
                std::cerr << "Invalid time input" << std::endl;
                return mktime(&currentTime);
            }
            currentTime.tm_hour = hour;
            currentTime.tm_min = minute;
            currentTime.tm_sec = second;
            time_correct = true;
        }
        else
        {
            std::cerr << "Failed to parse date and time." << std::endl;
        }
    }
    return mktime(&currentTime);
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
            std::cerr << "Parameter missing" << std::endl;
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
                std::cerr << "Task: Name is too long" << std::endl;
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
                std::cerr << "Task: Content is too long" << std::endl;
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
            const char *Con = (*it).c_str();
            bool time_correct = false;
            std::string str(Con);
            t.begin = convertToTimeT(str,time_correct);
            if(!time_correct) return false;
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
            const char *Con = (*it).c_str();
            bool time_correct = false;
            std::string str(Con);
            t.end = convertToTimeT(str,time_correct);
            if(!time_correct) return false;
            end_set = true;
        }

        else if(*it == "-r")
        {
             if(++it == words.end()) 
            {
                invalidCommand(iss);
                return false;
            }
            const char *Con = (*it).c_str();
            bool time_correct = false;
            std::string str(Con);
            t.remind.t = convertToTimeT(str,time_correct);
            if(!time_correct) return false;
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
            if(!strcasecmp(Con, "Low"))
                t.priority = Task::Low;
            else if(!strcasecmp(Con, "Mid"))
                t.priority = Task::Mid;
            else if(!strcasecmp(Con, "High"))
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
                std::cerr << "Tag is too long" << std::endl;
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
            if(!strcasecmp(Con, "unfin"))
                t.status = Task::Unfinished;
            else if(!strcasecmp(Con, "fin"))
                t.status = Task::Finished;
            else if(!strcasecmp(Con, "abort"))
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
    return true;
}

//parse taskID
bool parseTaskID(std::istringstream &iss, Tasks &using_tasks, uint64_t &id)
{
    iss >> id;
    if(iss.fail())
    {
        invalidCommand(iss);
        return false;
    }
    else if(id >= using_tasks.size() || using_tasks[id].status == Task::Abort)
    {
        std::cerr << "Tasks: No such task" << std::endl;
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
        std::cerr << "signin: Wrong password" << std::endl;
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
    uint64_t id;
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
    uint64_t id;
    if(!parseTaskID(iss, using_tasks, id)) return 1;
    if(!parseTask(iss, using_tasks[id])) return 1;
    return 0;
}

int showtask_aux(const std::vector<uint64_t> &v, Tasks &using_tasks)
{
    if(v.empty())
    {
        std::cout << "showtask: No tasks" << std::endl;
        return 1;
    }
    std::cout << std::left << std::setw(8) << "taskID" << " ";
    std::cout << std::left << std::setw(TASKNAME_SIZE) << "name";
    std::cout << std::left << std::setw(21) << "begin time";
    std::cout << std::left << std::setw(21) << "end time";
    std::cout << std::left << std::setw(21) << "remind time";
    std::cout << std::left << std::setw(5) << "pri";
    std::cout << std::left << std::setw(6) << "stat";
    std::cout << std::left << std::setw(TAGNAME_SIZE) << "tag" << std::endl;
    std::cout << "\t" << "content" << std::endl;

    for(auto i : v)
    {
        if(using_tasks[i].status == Task::Abort) continue;
        std::cout << std::left << std::setw(8) << i << " ";
        using_tasks[i].showtask();
    }
    return 0;
}

int show_high_pri (Tasks& using_tasks)
{
    std::cout << "Tasks with high priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::High);
    return showtask_aux(v, using_tasks);
}

int show_mid_pri (Tasks& using_tasks)
{
    std::cout << "Tasks with middle priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::Mid);
    return showtask_aux(v, using_tasks);
}

int show_low_pri (Tasks& using_tasks)
{
    std::cout << "Tasks with low priority: " << std::endl;
    auto v = using_tasks.select(&Task::priority, Task::Low);
    return showtask_aux(v, using_tasks);
}

int show_by_tag(Tasks& using_tasks,std::string &word)
{
    std::cout << "Tasks with tag: " << word << std::endl;
    auto v = using_tasks.select([&](const Task &x) -> bool
        {
            int i = 0;
            for(; x.tags[i] != '\0' && word[i] != '\0'; ++i)
            {
                if (x.tags[i] != word[i]) return false;
            }
            if (x.tags[i] != '\0' || word[i] != '\0') return false;
            return true;
        });
    return showtask_aux(v, using_tasks);
}

bool show_by_day(int days,const Task &using_task)
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

int show_day(Tasks& using_tasks,int days)
{
    std::cout << "Tasks in " << days << "days: " << std::endl;
    auto v = using_tasks.select([&days](const Task &x) -> bool
    {
        return show_by_day(days,x);
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
    if(!iss.eof())
    {
        if(word == "-p")
        {
            iss >> word;
            if(!iss.eof())
                return invalidCommand(iss);
            if(strcasecmp(word.c_str(),"high") == 0)
                return show_high_pri(using_tasks);
            else if(strcasecmp(word.c_str(),"mid") == 0)
                return show_mid_pri(using_tasks);
            else if(strcasecmp(word.c_str(),"low") == 0)
                return show_low_pri(using_tasks);
            else 
            {
            std::cerr << "No such priority" << std::endl; 
            return 1; 
            }
        }
        else if(word == "-t")
        {
            iss >> word;
            if(!iss.eof())
                return invalidCommand(iss);
            return show_by_tag(using_tasks,word);
        }
        else if(word == "-d")
        {
            int days = 0;
            iss >> days;
            if(iss.fail() || !iss.eof())
                return invalidCommand(iss);
            return show_day(using_tasks,days);
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
        return show_day(using_tasks,0);

    else
        return invalidCommand(iss);
    return 0;
}
