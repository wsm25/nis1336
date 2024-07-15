/* Schedule Command Line Tool
 *
 * Usage: schedule-cli run                                  run shell with schedule reminder
 *        schedule-cli <username> <password> <command>      run <command> with given user <user>
 *        schedule-cli help                                 print this help
 *
 * Commands:
 * - addtask [arguments]                    add task to schedule. no repaet begin time
 *   Arguments:
 *    -n <name>             set task name. no more than TASKNAME_SIZE. default: "new task"
 *    -b <time>             set begin time. <time>: [YYYY/MM/DD]-HH:MM:SS. default: now
 *    -e <time>             set end time. default: begin time
 *    -r <time>             set remind time. default: begin time
 *    -p low|middle|high    set priority. default: none
 *    -c <content>          set content. no more than TASKCONTENT_SIZE. default: ""
 *    -t tag                set tags. no more than MAX_TAGS_PER_TASK. default: null
 *    -f                    overwrite if task exists
 *
 * - showtask [arguments]                   show task with given filter and given order
 *   Arguments:
 *    -b HH:MM:SS           earliest task time. default: 00:00:00
 *    -e HH:MM:SS           latest task time. default: 23:59:59
 *    -d <days>             show tasks in next n days. default: 1(today)
 *    -p pri                show task with given priority. default: all
 *    -t tag                show tasks with given tags. default: null
 *    -o <order>            show tasks with given order. <order>:
 *
 * - edittask <taskID> [arguments]          edit task with given ID
 *   Arguments:
 *    -n <name>                     edit task name. no more than TASKNAME_SIZE
 *    -b <time>                     edit begin time. <time>: [YYYY/MM/DD] HH:MM:SS. no repaet begin time
 *    -e <time>                     edit end time
 *    -r <time>                     edit remind time
 *    -p low|middle|high            edit priority
 *    -c <content>                  edit content. no more than TASKCONTENT_SIZE
 *    -t tag1,tag2,..               edit tags. no more than MAX_TAGS_PER_TASK
 *    -s unfinished|finished|abort  edit status
 *
 * - deltask <taskID>                       delete task with given ID. alias for `edittask <taskID> -s abort`
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "Command.h"

 ///线程函数
void *Input(void *arg)
{
    std::string command;
    Tasks *using_tasks = (Tasks *)arg;
    while(true)
    {
        std::cout << "(schedule) ";
        std::cin >> command;

        if(!strcasecmp(command.c_str(), "addtask"))
            addtask(using_tasks);
        else if(!strcasecmp(command.c_str(), "showtask"))
            showtask(using_tasks);
        else if(!strcasecmp(command.c_str(), "edittask"))
            edittask(using_tasks);
        else if(!strcasecmp(command.c_str(), "deltask"))
            deltask(using_tasks);
        else
            std::cerr << "invalid command" << std::endl;
    }
    return (void *)0;
}

void *Remind(void *arg)
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

int main(int argc, char *argv [])
{
    if(argc < 2 || (argc > 2 && argc <= 4) || (argc == 2 && strcmp(argv[1], "run") != 0))
    {
        std::cerr << "Invalid Input" << std::endl;
        return 1;
    }

    //case：schedule-cli [username] [password] [command]    run one command with given user
    if(argc > 4)
    {
        char *using_user = argv[1];
        char *input_password = argv[2];
        Storage opening_file(using_user);
        if(!Open_By_Username(using_user, input_password, opening_file))
            return 0;
        char *command = argv[3];
        ///TODO：command we want


        return 0;
    }


    //case: schedule-cli run--run shell with schedule reminder
    ///输入用户名
    std::cout << "Please enter your username: " << std::endl;
    std::string user_name, your_password;
    std::cin >> user_name;
    std::cout << "Enter your password: " << std::endl;
    std::cin >> your_password;
    Storage using_file(user_name.c_str());
    if(!Open_By_Username(user_name.c_str(), your_password.c_str(), using_file))
        return 0;

    Tasks tasks(using_file);


    ///创建输入线程&&定时提示线程
    pthread_t command_thread, remind_thread;

    ///取消cin与cout的同步
    std::ios::sync_with_stdio(false);

    if(pthread_create(&command_thread, NULL, Input, (void *)&tasks)) {
        std::cerr << "Fail to creat COMMAND_THREAD" << std::endl;
        return -1;
    }

    if(pthread_create(&remind_thread, NULL, Remind, (void *)&tasks)) {
        std::cerr << "Fail to creat WARNING_THREAD" << std::endl;
        return -1;
    }

    ///等待线程结束,释放线程的资源 
    pthread_join(command_thread, NULL);
    pthread_join(remind_thread, NULL);

    std::cout << "Control thread id: " << pthread_self() << std::endl;
    std::cout << "finished!\n";

    return 0;
}
