#ifndef COMMAND_H
#define COMMAND_H


#include<iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <unordered_map>
#include "tasks.h"
#include "unistd.h"


// 解析包含日期和时间的字符串为tm结构
inline std::tm parseDateTime(const std::string& dateTimeStr)
{
    std::tm tmTime = {};
    std::istringstream iss(dateTimeStr);
    char discard; // 用于丢弃分隔符
    int year, month, day;
    int hour, minute, second;

    if (iss >> year >> discard >> month >> discard >> day >> 
        discard >> hour >> discard >> minute >> discard >> second)
    {
        tmTime.tm_year = year - 1900;
        tmTime.tm_mon = month - 1;
        tmTime.tm_mday = day;
        tmTime.tm_hour = hour;
        tmTime.tm_min = minute;
        tmTime.tm_sec = second;
    }
     else 
    {
        std::cerr << "Failed to parse date and time." << std::endl;
    }

    return tmTime;
}

// 将输入字符串转换为time_t类型
inline time_t convertToTimeT(const std::string& input) 
{
    std::tm currentTime = {};
    if (input.find('/') != std::string::npos) // 包含日期
    { 
        currentTime = parseDateTime(input);
    } 
    else // 仅时间，使用当前日期
    {   
        // 将time_t类型的时间转换为tm结构，考虑本地时区
        time_t rawTime = time(0);
        currentTime = *localtime(&rawTime); // 获取当前时间的tm结构
        int hour, minute, second;
        char discard; // 用于丢弃分隔符

        std::istringstream iss(input);
        if (iss >> hour >> discard >> minute >> discard >> second)
        {
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


inline void quit(std::istringstream &iss, bool &flag, Storage &using_file)
{
    flag = false;
}

inline void addtask(std::istringstream &iss, bool &flag, Storage &using_file)
{
   
    std::string word;
    // 用来存储分割后的单词
    std::vector<std::string> words;
    Task t;
    //防止先输入-e或者-r,再输入-b将其覆盖
    bool end_set = false;
    bool remind_set = false;
     // 循环读取每个单词
    while (iss >> word) 
        words.push_back(word);

    int i = 0;
    auto it = words.begin() + i;
    if(it == words.end()) 
    {
        std::cerr << "Wrong Input" << std::endl;
        return;
    }
    while(it != words.end())
    {   
        //检查指令后是否有参数
        if(it == words.end())
        {
        std::cerr << "Wrong Input" << std::endl;
        return;
        }
        //此时it为输入参数
        i++;
        const char* Con = words[i].c_str();
        if(*it == "-n")
        {
            it++;
            t.name = Con;
        }

        else if (*it == "-c")
        { 
            it++;
            if(std::strlen(Con) >= TASKCONTENT_SIZE)
            {
                std::cerr << "Length is out of range" << std::endl;
                return;
            }
            int i = 0;
            for (; Con[i] != '\0'; ++i)
                t.content[i] = Con[i];
            t.content[i] = '\0';
        }
        
        else if (*it == "-b")
        {
            it++;
            std::string str(Con);
            t.begin = convertToTimeT(str);
            if(!end_set)
                t.end = t.begin;
            if(!remind_set)
                t.remind.t = t.begin;
        }

        else if (*it == "-e")
        {
            it++;
            std::string str(Con);
            t.end = convertToTimeT(str);
            end_set = true;
        }

        else if (*it == "-r")
        {
            it++;
            std::string str(Con);
            t.remind.t = convertToTimeT(str);
            remind_set = true;
        }

        else if(*it == "-p")
        {
            it++;
            if(!strcmp(Con,"Low")) 
                t.priority = Task::Low;
            else if(!strcmp(Con,"Mid"))
                t.priority = Task::Mid; 
            else if(!strcmp(Con,"High"))
                t.priority = Task::High; 
        }

        else if(*it == "-t")
        {
            it++;
            if(strlen(Con) >= TAGNAME_SIZE)
            {
                std::cerr << "Tag is too long" << std::endl;
                return;
            }
            int j= 0;
            for(; Con[j] != '\0' ; ++j)
                t.tags[j] = Con[j];
            t.tags[j] = '\0';
        }
        else
        {
            std::cerr << "Wrong command" << std::endl;
            return;
        }

        if(it == words.end()) break;
        it++;
        i++;
    }//while结束
    using_file.insert_task(t);
    return;
}

inline void deltask(std::istringstream &iss, bool &flag, Storage &using_file)
{
    int id;
    iss >> id;
    Tasks using_tasks(using_file);
    using_tasks[id].status = Task::Abort;
    return;
}

//show by specific order
inline void showtask(std::istringstream &iss, bool &flag, Storage &using_file)
{
    std::string word;
    iss >> word;
    Tasks using_tasks(using_file);

    if(word == "-r")
    {
        auto v = using_tasks.sort(&Task::remind);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << using_tasks[i].name << std::endl;
        }
    }

    else if(word == "-b")
    {
        auto v = using_tasks.sort(&Task::begin);
        for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << using_tasks[i].name << std::endl;
        }
    }

    else if(word == "-p")
    {
        auto v = using_tasks.sort(&Task::begin);
         for(auto i : v)
        {
            if(using_tasks[i].status == Task::Unfinished && !using_tasks[i].remind.isReminded)
                std::cout << using_tasks[i].name << std::endl;
        }
    }

    else
    {
        std::cerr << "Wrong command" << std::endl;
        return;
    }

    return;
}

inline void selecttask(std::istringstream &iss, bool &flag, Storage &using_file)
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
        return;
    }

    return;
}

//第一次打开文件的操作
inline bool Open_By_Username(const char* user_name,const char* your_password,Storage &using_file)
{
    using_file.user();
    if(using_file.fail())
    {
        if(using_file.user().set_password(your_password))
        {
            std::cout << "Create your account successfully" << std::endl;
            return true;
        }
        std::cout << "Invalid password" << std::endl;
        return false;
    }

    else if(!using_file.user().verify_password(your_password))
    {
        std::cout << "Your password is wrong" << std::endl;
        return false;
    }

    else
    {
        std::cout << "Open your account successfully!" << std::endl;
        return true;
    }
}


inline void *Remind(void *arg)
{
    //参数强转成Tasks类
    Tasks *using_tasks = (Tasks *)arg;
    while(true)
    {
        auto v = using_tasks->select(
            [&](Task &task) -> bool {
                return (task.status == Task::Unfinished && task.remind.check());
            }
        );

        //输出任务提醒
        Task remind_task;
        for(auto i : v)
        {
            remind_task = (*using_tasks)[i];
            std::cout << "Time for: " << remind_task.name << std::endl;
        }
        sleep(5);
    }

    return (void *)0;
}

void shell();

// 命令数组
const std::unordered_map<std::string, void(*)(std::istringstream &, bool &, Storage &)> cmds = {{"quit", quit}, {"addtask", addtask}, {"showtask", showtask}, {"deltask", deltask}};

#endif