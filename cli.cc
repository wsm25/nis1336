/* Schedule Command Line Tool
 *
 * Usage: schedule-cli run                                run shell with schedule reminder
 *    or: schedule-cli [username] [password] [command]    run one command with given user
 * 
 * Commands:
 * - addtask [name] HH:MM:SS [arguments]      add task to schedule with notice time. name cannot repeat
 *   Arguments:
 *       -s HH:MM:SS          set start time. default: notice time
 *       -d YY/MM/DD          set date. default: today
 *       -p low|middle|high   set priority. default: none
 *       -c <content>         set content. default: ""
 *       -t tag1,tag2,..      set tags. default: null
 *       -f                   overwrite if task exists
 * 
 * - showtask [name]                          show task with given name
 * - list [arguments]                         list tasks with given filter
 *   Arguments:
 *       -s HH:MM:SS          earliest task time. default: 00:00:00
 *       -e HH:MM:SS          latest task time. default: 23:59:59
 *       -d <days>            list tasks in next n days. default: 1(today)
 *       -p pri1,pri2..       list task with given priority. default: all
 *       -t tag1,tag2,..      list tasks with given tags. default: null
 * 
 * - deltask [name]                          delete task with given name
 */


#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<pthread.h>
#include <unistd.h>
#include<string>
#include<cstring>
#include <ctime>
#include"storage.h"
#include"tasks.h"
#include"Command.h"



///线程函数
void* Input(void*arg)
{
    std::string command;

    while (true)
    {
        std::cin >> command;
        
        if(strcasecmp(command.c_str(),"addtask"))
        {
            addtask();
        }
    }
    

    return (void*)0;
}

void* Remind(void*arg)
{
    while(true)
    {
        // 获取当前时间
        time_t now;
        time(&now);

        //参数强转成Tasks类
        Tasks *using_tasks = (Tasks*)arg;
        auto v = using_tasks->select(
            [&](const Task &task) -> bool{
                return task.remind == now;
            }
        );

        //输出任务提醒
        Task remind_task;
        int v_size =v.size();
        for(int i = 0; i < v_size; ++i)
        {
            remind_task = (*using_tasks)[v[i]];
            std::cout << "Time for: " <<remind_task.name << std::endl;
        }
        sleep(5);
    }

    return (void*) 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2 || (argc > 2 && argc <= 4) ||(argc == 2 && strcmp(argv[1],"run") != 0))
    {
        std::cerr << "Invalid Input" << std::endl;
        return 1;
    }

    //case：schedule-cli [username] [password] [command]    run one command with given user
    if(argc > 4)
    {
        char* using_user = argv[1];
        char* input_password = argv[2];
        Storage opening_file(using_user);
        if(!Open_By_Username(using_user,input_password,opening_file))
        {
            delete[]using_user;
            delete[]input_password;
            return 0;
        } 
        char* command = argv[3];
        ///TODO：command we want


        return 0;
    }


    //case: schedule-cli run--run shell with schedule reminder
    ///输入用户名
    std::cout << "Please enter your username: " << std::endl;
    std::string user_name,your_password;
    std::cin >> user_name;
    std::cout << "Enter your password: " << std::endl;
    std::cin >> your_password;
    Storage using_file(user_name.c_str());
    if(!Open_By_Username(user_name.c_str(),your_password.c_str(),using_file))
        return 0;

    Tasks tasks(using_file);


    ///创建输入线程&&定时提示线程
    pthread_t command_thread,remind_thread;

    if(pthread_create(&command_thread,NULL,Input,NULL)) { 
        std::cerr << "Fail to creat COMMAND_THREAD" << std::endl; 
        return -1; 
    } 
    
    if(pthread_create(&remind_thread,NULL,Remind,(void*)&tasks)) { 
        std::cerr << "Fail to creat WARNING_THREAD" << std::endl; 
        return -1; 
    } 

    ///等待线程结束,释放线程的资源 
    pthread_join(command_thread,NULL); 
    pthread_join(remind_thread,NULL); 

    printf("Control thread id: %lx\n",pthread_self());
    printf("finished!\n");

    return 0;
}
