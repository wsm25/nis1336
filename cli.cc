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

void* Warning(void*arg)
{
    // for(int i = -10; i < 0; ++i)
    // {
    //     printf("%lx WARING_running %d\n",pthread_self(),i);
    //     int time = (int)(drand48() * 100000);
    //     usleep(time);
    // }

    // return (void*)0;
}

int main(int argc, char* argv[])
{
    ///

    ///创建输入线程&&定时提示线程
    pthread_t add_task_thread,warning_thread;

    if(pthread_create(&add_task_thread,NULL,Input,NULL)) { 
        std::cerr << "Fail to creat INPUT_THREAD" << std::endl; 
        return -1; 
    } 
    
    if(pthread_create(&warning_thread,NULL,Warning,NULL)) { 
        std::cerr << "Fail to creat WARNING_THREAD" << std::endl; 
        return -1; 
    } 

    ///等待线程结束,释放线程的资源 
    pthread_join(add_task_thread,NULL); 
    pthread_join(warning_thread,NULL); 

    printf("Control thread id: %lx\n",pthread_self());
    printf("finished!\n");

    return 0;
}
