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

int main(int argc, char* argv[]){}