#include<iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include"storage.h"
#include"tasks.h"

// 解析包含日期和时间的字符串为tm结构
std::tm parseDateTime(const std::string& dateTimeStr)
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
time_t convertToTimeT(const std::string& input) 
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


void addtask(Storage &using_file)
{
   std::cout << "Enter your task: " << std::endl;
   std::string inputLine;
   // 使用getline读取整行输入，包括空格
   getline(std::cin, inputLine);
    // 当用户仅按回车没有输入内容时，inputLine将为空，这里作为退出条件处理
    if (inputLine.empty()) 
    {
        std::cout << "未输入任何内容。" << std::endl;
        return ;
    }
    // 使用istringstream来分割字符串
    std::istringstream iss(inputLine);
    std::string word;
    // 用来存储分割后的单词
    std::vector<std::string> words;
    Task t;
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
        if (*it == "-n")
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
        //TODO
        if (*it == "-b")
        {
            i++;
            const char* Con = words[i].c_str();
            char*date = new char[25];
            for (int j = 0;Con[j] != '\0'; ++j)
                date[j] = Con[j];
            date[10] = ' ';

        }

        if(*it == "-p")
        {
            i++;
            const char* Con = words[i].c_str();
            if(!strcmp(Con,"Low")) 
                t.priority = Task::Low;
            else if(!strcmp(Con,"Mid"))
                t.priority = Task::Mid; 
            else if(!strcmp(Con,"High"))
                t.priority = Task::High; 
        }

        if(*it == "-t")
        {
            i++;
            const char* Con = words[i].c_str();
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

        delete[]Con;
        if(it == words.end()) break;
        it++;
        i++;
    }//while结束
    return;
}

void deltask(Storage &using_file)
{
    std::cout << "Enter your task ID: " << std:: endl;
    int id;
    std::cin >> id;
    Tasks using_tasks(using_file);
    Task *arr = using_tasks.data();
    arr[id].status = Task::Abort;
    return;
}

//show by remind_time
void showtask(Storage &using_file)
{
    Tasks using_tasks(using_file);
    uint64_t len;
    Task *arr = using_file.tasks(len);
    auto v = using_tasks.sort(&Task::remind);
    int i = 0;
    int length = v.size();
    for(;i < length; ++ i)
    {
        if(arr[v[i]].status == Task::Unfinished && !arr[v[i]].remind.isReminded)
            std::cout << arr[v[i]].name << std::endl;
    }
    return;
}

//第一次打开文件的操作
bool Open_By_Username(const char* user_name,const char* your_password,Storage &using_file)
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

    else if(using_file.user().verify_password(your_password))
    {
        std::cout << "Open your account successfully!" << std::endl;
        return true;
    }

    Tasks tasks(using_file);
}