#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <utility>


// auxiliary function

// error message
int invalidCommand(std::istringstream &iss)
{
    std::cerr << "Invalid command: " << iss.str() << "\ntry help" << std::endl;
    return 1;
}

// parsing
// parse username with hint
void parseUsernameHint(std::istringstream &iss, std::string &username)
{
    if(iss.eof())
    {
        std::cout << "Username: ";
        getline(std::cin, username);
    }
    else
        iss >> username;
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
bool parseDateTime(const std::string &dateTimeStr, tm &tmTime)
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
    }
    else
    {
        std::cerr << "Time: Failed to parse time." << std::endl;
    }

    return true;
}

// convert string to time_t
time_t convertToTimeT(const std::string &input)
{
    std::tm currentTime = {};
    if(input.find('/') != std::string::npos) // 包含日期
    {
        parseDateTime(input, currentTime);
    }
    else // 仅时间，使用当前日期
    {
        // 将time_t类型的时间转换为tm结构，考虑本地时区
        time_t rawTime = time(0);
        currentTime = *localtime(&rawTime); // 获取当前时间的tm结构
        int hour, minute, second;
        char discard; // 用于丢弃分隔符

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
            std::cerr << *it << ": Parameter missing" << std::endl;
            invalidCommand(iss);
            return false;
        }
        //此时it为输入参数
        i++;
        const char *Con = words[i].c_str();
        if(*it == "-n")
        {
            it++;
            t.name = Con;
        }

        else if(*it == "-c")
        {
            it++;
            if(std::strlen(Con) >= TASKCONTENT_SIZE)
            {
                std::cerr << "Length is out of range" << std::endl;
                return false;
            }
            int i = 0;
            for(; Con[i] != '\0'; ++i)
                t.content[i] = Con[i];
            t.content[i] = '\0';
        }

        else if(*it == "-b")
        {
            it++;
            std::string str(Con);
            t.begin = convertToTimeT(str);
            if(!end_set)
                t.end = t.begin;
            if(!remind_set)
                t.remind.t = t.begin;
        }

        else if(*it == "-e")
        {
            it++;
            std::string str(Con);
            t.end = convertToTimeT(str);
            end_set = true;
        }

        else if(*it == "-r")
        {
            it++;
            std::string str(Con);
            t.remind.t = convertToTimeT(str);
            remind_set = true;
        }

        else if(*it == "-p")
        {
            it++;
            if(!strcmp(Con, "Low"))
                t.priority = Task::Low;
            else if(!strcmp(Con, "Mid"))
                t.priority = Task::Mid;
            else if(!strcmp(Con, "High"))
                t.priority = Task::High;
        }

        else if(*it == "-t")
        {
            it++;
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
    parseUsernameHint(iss, username);
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
    parseUsernameHint(iss, username);
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
    parseUsernameHint(iss, username);
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

int signout(std::istringstream &iss, Storage &using_file)
{
    if(iss.eof())
    {
        using_file.signout();
        return 0;
    }
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
        return using_tasks.insert(t);
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

int showtask(std::istringstream &iss, Tasks &using_tasks)
{
    std::string word;
    iss >> word;
    if(!iss.eof() && word != "-o")
        return invalidCommand(iss);

    if(word == "-r")
    {
        auto v = using_tasks.sort(&Task::remind);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << i << using_tasks[i].name << std::endl;
        }
    }

    else if(word == "-b")
    {
        auto v = using_tasks.sort(&Task::begin);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << i << using_tasks[i].name << std::endl;
        }
    }

    else if(word == "-p")
    {
        auto v = using_tasks.sort(&Task::begin);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << i << using_tasks[i].name << std::endl;
        }
    }

    else
        return invalidCommand(iss);


    return 0;
}

int selecttask(std::istringstream &iss, bool &flag, Storage &using_file)
{
    std::string word;
    iss >> word;
    Tasks using_tasks(using_file);

    //-hp show tasks with high priority
    if(word == "-hp")
    {
        auto v = using_tasks.select(&Task::priority, Task::High);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << using_tasks[i].name << std::endl;
        }
    }
    //-uf show tasks that is Unfinushed
    else if(word == "-uf")
    {
        auto v = using_tasks.select(&Task::status, Task::Unfinished);
        for(auto i : v)
        {
            if(!using_tasks[i].remind.isReminded)
                std::cout << using_tasks[i].name << std::endl;
        }
    }

    else
    {
        std::cerr << "Wrong command" << std::endl;
        return 1;
    }

    return 0;
}

void *remind(void *args)
{
    // find tasks to remind
    Tasks *using_tasks = ((std::pair<Tasks *, bool *> *)args)->first;
    bool *isstopped = ((std::pair<Tasks *, bool *> *)args)->second;
    
    while(true)
    {
        auto v = using_tasks->select(
            [&](const Task &task) -> bool {
                return (task.status == Task::Unfinished && task.remind.check());
            }
        );

        // print remind
        for(auto i : v)
        {
            Task &remind_task = (*using_tasks)[i];
            std::cout << "\n\nTime for: " << remind_task.name << std::endl;
            remind_task.remind.isReminded = true;
        }
        // sleep(5); // avoid blocking
        if(*isstopped) break;
    }

    return nullptr;
}