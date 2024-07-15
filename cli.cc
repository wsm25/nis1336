/* Schedule Command Line Tool
 *
 * Usage: schedule-cli run                                  run shell with schedule reminder
 *        schedule-cli <username> <password> <command>      run <command> with given user <user>
 *        schedule-cli help                                 print this help
 *
 * Commands:
 * - signup                                 sign up a new account with given <username> and <password>
 * 
 * - editname <new username>                edit username to <new username>
 * 
 * - editpwd <new password>                 edit password to <new password>
 * 
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
