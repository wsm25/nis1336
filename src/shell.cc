#include "shell.h"

#include <pthread.h>
extern pthread_mutex_t lock;

///static member
//terminal
std::istringstream terminal::iss;
Storage terminal::using_file;
Tasks terminal::using_tasks(terminal::using_file);

const terminal::CMDS terminal::cmds = {{"editname", editname}, {"editpwd", editpwd}, {"cancel", cancel}, 
        {"addtask", addtask}, {"edittask", edittask}, {"showtask", showtask}, {"deltask", deltask}};

//schedule
std::istringstream schedule::iss;
bool schedule::isstopped;

const schedule::CMDS schedule::cmds = {{"help", help}, {"quit", quit}, 
    {"signin", signin}, {"signup", signup}};

//user
std::istringstream user::iss;
bool user::isstopped;
Storage user::using_file;
Tasks user::using_tasks(user::using_file);
pthread_t user::remind_thread;

const user::CMDS user::cmds = {{"help", help}, {"signout", signout}, {"quit", quit}, 
    {"editname", editname}, {"editpwd", editpwd}, {"cancel", cancel},
    {"addtask", addtask}, {"edittask", edittask}, {"showtask", showtask}, {"deltask", deltask}};

///shell
//terminal
int terminal::shell(int argc, char *argv [])
{
    //signin
    if(signin(argv[1], argv[2]))
        return invalidCommand(argc, argv);

    std::string inputLine(argv[3]);
    for(int i = 4; i < argc; ++i)
    {
        inputLine += ' ';
        inputLine += argv[i];
    }

    iss.str(inputLine);
    iss.clear();
    std::string command;
    iss >> command;

    CMDS::const_iterator it = cmds.find(command);
    if(it != cmds.end())
        return it->second();
    else
        return invalidCommand(argc ,argv);
}

//schedule
int schedule::shell()
{
    iss.setstate(iss.eofbit); help();
    isstopped = false;
    while(true)
    {
        std::cout << "(schedule) ";

        std::string inputLine;
        getline(std::cin, inputLine);
        if(inputLine.empty()) continue;

        iss.clear();
        iss.str(inputLine);
        std::string command;
        iss >> command;

        CMDS::const_iterator it = cmds.find(command);
        if(it == cmds.end())
            ::invalidCommand(iss);
        else
            it->second();
        if(isstopped) return 0;
    }
    return 1;
}

//user
int user::shell()
{
    isstopped = false;

    ///asynchronize std::cin and std::cout
    std::ios::sync_with_stdio(false);

    pthread_mutex_init(&lock, 0);

    if(errno = pthread_create(&remind_thread, NULL, remind, NULL))
    {
        perror("remind thread: ");
        return 1;
    }

    std::cout << "Welcome: " << using_file.user().Name() << std::endl;
    iss.setstate(iss.eofbit); showtask();
    while(true)
    {
        std::cout << "(" << using_file.user().Name() << ") ";
        std::string inputLine;
        getline(std::cin, inputLine);
        if(inputLine.empty()) continue;

        iss.str(inputLine);
        iss.clear();
        std::string command;
        iss >> command;

        CMDS::const_iterator it = cmds.find(command);
        if(it == cmds.end())
            ::invalidCommand(iss);
        else
            it->second();
        if(isstopped) return 0;
    }
    return 1;
}

///shell-related command
//terminal
int terminal::help()
{
    std::cout << HELP_TERMINAL << std::endl;
    return 0;
}

int terminal::invalidCommand(int argc, char *argv [])
{
    std::cerr << RED << "Invalid command:";
    for(int i = 1; i < argc; ++i)
        std::cerr << " " << argv[i];
    std::cerr << "\ntry help" << RESET << std::endl;
    return 1;
}

int terminal::signup(const char *user_name, const char *password)
{
    User user;
    if(!user.set_username(user_name)) return 1;
    if(!user.set_password(password)) return 1;
    Storage tmp(user);
    return tmp.fail();
}

int terminal::signin(const char *user_name, const char *password)
{
    using_file.signin(user_name);
    if(using_file.fail()) return 1;
    if(!using_file.user().verify_password(password))
    {
        using_file.signout();
        std::cerr << RED << "signin: Wrong password" << RESET << std::endl;
        return 1;
    }
    return 0;
}

int terminal::editname() { return ::editname(iss, using_file); }
int terminal::editpwd() { return ::editpwd(iss, using_file); }
int terminal::cancel() { return ::cancel(iss, using_file); }
int terminal::addtask() { return ::addtask(iss, using_tasks); }
int terminal::edittask() { return ::edittask(iss, using_tasks); }
int terminal::showtask() { return ::showtask(iss, using_tasks); }
int terminal::deltask() { return ::deltask(iss, using_tasks); }

//schedule
void schedule::quit()
{
    if(iss.eof())
        isstopped = true;
    else
        ::invalidCommand(iss);
}

void schedule::help()
{
    if(iss.eof())
        std::cout << HELP_SCHEDULE << std::endl;
    else
        ::invalidCommand(iss);
}

void schedule::signin()
{
    if(::signin(iss, user::using_file) == 0)
        user::shell();
}

void schedule::signup()
{
    if(::signup(iss, user::using_file) == 0)
        user::shell();
}

//user
void user::help()
{
    if(iss.eof())
        std::cout << HELP_USER << std::endl;
    else
        ::invalidCommand(iss);
}

void user::quit()
{
    if(iss.eof())
    {
        isstopped = true;
        pthread_join(remind_thread, NULL);
        using_file.signout();
        schedule::isstopped = true;
    }
    else
        invalidCommand(iss);
}

void user::signout()
{
    if(iss.eof())
    {
        isstopped = true;
        pthread_join(remind_thread, NULL);
        using_file.signout();
    }
    else
        invalidCommand(iss);
}

void user::cancel()
{
    if(iss.eof())
    {
        isstopped = true;
        pthread_join(remind_thread, NULL);
        using_file.cancel();
    }
    else
        invalidCommand(iss);
}

void *user::remind(void *args)
{
    while(true)
    {
        pthread_mutex_lock(&lock);
        auto v = using_tasks.select(
            [&](const Task &task) -> bool {
                return (task.status == Task::Unfinished && task.remind.check());
            }
        );

        // print remind
        for(auto i : v)
        {
            Task &remind_task = using_tasks[i];
            std::cout << "\n\nTime for: " << i << " " << remind_task.name << "\n\a" << std::endl;
            remind_task.remind.isReminded = true;
        }
        pthread_mutex_unlock(&lock);
        if(isstopped) break;
        sleep(1);
    }

    return nullptr;
}

void user::editname() { ::editname(iss, using_file); }
void user::editpwd() { ::editpwd(iss, using_file); }
void user::addtask() { ::addtask(iss, using_tasks); }
void user::edittask() { ::edittask(iss, using_tasks); }
void user::showtask() { ::showtask(iss, using_tasks); }
void user::deltask() { ::deltask(iss, using_tasks); }
