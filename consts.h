#ifndef SCHEDULE_CONSTS_H
#define SCHEDULE_CONSTS_H

#define TAGNAME_SIZE 16
#define USERNAME_SIZE 20
#define TASKNAME_SIZE 256
#define TASKCONTENT_SIZE 1024

#define MAX_TAGS_PER_TASK 16
#define MAX_TAGS_PER_USER 128

#define HASHEDPASSED_SIZE 128
#define PASSWORD_SIZE 20

#define DATADIR "data/"
#define FILEPATH_SIZE 44 // (DATADIR_SIZE - 1) + 2 * (USERNAME_SIZE - 1) + 1

constexpr char HELP_TERMINAL[] = 
R"WSM(Schedule Command Line Tool

Usage:  schedule run                                      run shell with schedule reminder
        schedule <username> <password> <command>          run <command> with given user <user>
        schedule help                                     print this help

Commands:
 signup                                 sign up a new account with <username> and <password>

 editname [<new username>]              edit username to <new username>. 
                                        default: program will guide you to enter new username

 editpwd [<new password>]               edit password to <new password>. 
                                        default: program will give you a hint to secretly enter new password

 cancel                                 cancel the account

 addtask [arguments]                    add task to schedule. no repaet begin time
  Arguments:
    -n <name>               set task name. no more than TASKNAME_SIZE. default: "new task"
    -b <time>               set begin time. <time>: [YYYY/MM/DD-]HH:MM:SS. default: now
    -e <time>               set end time. default: begin time
    -r <time>               set remind time. default: begin time
    -p low|middle|high      set priority. default: none
    -c <content>            set content. no more than TASKCONTENT_SIZE. default: ""
    -t tag                  set tag. no more than TAGNAME_SIZE. default: null

 showtask [arguments]                   show task with given filter and given order
  Arguments:
   -b                       show tasks by begin time
   -e                       show tasks by end time
   -r                       show tasks by remind time
   -d <days>                show tasks begin in next n days. default: 1(today)
   -p <pri>                 show task with given priority. default: all
   -t <tag>                 show tasks with given tags. default: null

 edittask <taskID> [arguments]          edit task with given ID
  Arguments:
   -n <name>                     edit task name. no more than TASKNAME_SIZE
   -b <time>                     edit begin time. <time>: [YYYY/MM/DD] HH:MM:SS. no repaet begin time
   -e <time>                     edit end time
   -r <time>                     edit remind time
   -p low|middle|high            edit priority
   -c <content>                  edit content. no more than TASKCONTENT_SIZE
   -t tag                        edit tag. no more than TAGNAME_SIZE
   -s unfinished|finished|abort  edit status

 deltask <taskID>                delete task with given ID. alias for `edittask <taskID> -s abort`
 )WSM";

constexpr char HELP_SCHEDULE[] = 
R"HSY(Schedule Shell Interface

Usage:  help                                    print this help           
        quit                                    quit the shell
        signin [<username>] [<password>]        signin the account with <username> and <password>. 
                                                default: shell will guide you to enter
        signup [<username>] [<password>]        sign up a new account with <username> and <password>. 
                                                default: shell will guide you to enter
)HSY";

constexpr char HELP_USER [] =
R"PIPI(Schedule User Interface

Usage:  help                                    print this help           
        signout                                 sign out the account
        cancel                                  cancel the account
        editname [<new username>]               edit username to <new username>. 
                                                default: program will guide you to enter new username
        editpwd [<new password>]                edit password to <new password>. 
                                                default: program will give you a hint to secretly enter new password
        addtask [arguments]                     add task to schedule. no repaet begin time
         Arguments:
          -n <name>                 set task name. no more than TASKNAME_SIZE. default: "new task"
          -b <time>                 set begin time. <time>: [YYYY/MM/DD]-HH:MM:SS. default: now
          -e <time>                 set end time. default: begin time
          -r <time>                 set remind time. default: begin time
          -p low|middle|high        set priority. default: none
          -c <content>              set content. no more than TASKCONTENT_SIZE. default: ""
          -t tag                    set tag. no more than TAGNAME_SIZE. default: null

        showtask [arguments]                    show task with given filter and given order
         Arguments:
          -b                       show tasks by begin time
          -e                       show tasks by end time
          -r                       show tasks by remind time
          -d <days>                show tasks begin in next n days. default: 1(today)
          -p <pri>                 show task with given priority. default: all
          -t <tag>                   show tasks with given tags. default: null

        edittask <taskID> [arguments]           edit task with given ID
         Arguments:
          -n <name>                     edit task name. no more than TASKNAME_SIZE
          -b <time>                     edit begin time. <time>: [YYYY/MM/DD] HH:MM:SS. no repaet begin time
          -e <time>                     edit end time
          -r <time>                     edit remind time
          -p low|middle|high            edit priority
          -c <content>                  edit content. no more than TASKCONTENT_SIZE
          -t tag                        edit tag. no more than TAGNAME_SIZE
          -s unfinished|finished|abort  edit status

        deltask <taskID>            delete task with given ID. alias for `edittask <taskID> -s abort`
)PIPI";

#endif // SCHEDULE_CONSTS_H