#include "shell.h"

///shell
int schedule::shell()
{
    bool isstopped = false;
    while(true)
    {
        std::cout << "(schedule) ";

        std::string inputLine;
        getline(std::cin, inputLine);
        if(inputLine.empty()) continue;

        std::istringstream iss(inputLine);
        std::string command;
        iss >> command;

        CMDS::const_iterator it = cmds.find(command);
        if(it == cmds.end())
            ::invalidCommand(iss);
        else
            it->second(iss, isstopped);
        if(isstopped) return 0;
    }
    return 1;
}

int user::shell(Storage &using_file)
{
    Tasks using_tasks(using_file);

    pthread_t remind_thread;

    ///asynchronize std::cin and std::cout
    std::ios::sync_with_stdio(false);

    if(errno = pthread_create(&remind_thread, NULL, remind, (void *)&using_tasks))
    {
        perror("remind thread: ");
        return 1;
    }

    bool isstopped = false;
    while(true)
    {
        std::cout << "(" << using_file.user().Name() << ") ";
        std::string inputLine;
        getline(std::cin, inputLine);
        if(inputLine.empty()) continue;

        std::istringstream iss(inputLine);
        std::string command;
        iss >> command;

        CMDS::const_iterator it = cmds.find(command);
        if(it == cmds.end())
            ::invalidCommand(iss);
        else
            it->second(iss, isstopped, using_file, using_tasks);
        if(isstopped) return 0;
    }
    return 1;
}

///shell-related command
//schedule::shell
void schedule::quit(std::istringstream &iss, bool &isstopped)
{
    if(iss.eof())
        isstopped = true;
    else
        ::invalidCommand(iss);
}

void schedule::help(std::istringstream &iss, bool &isstopped)
{
    if(iss.eof())
        std::cout << HELP_SCHEDULE <<std::endl;
    else
        ::invalidCommand(iss);
}

void schedule::signin(std::istringstream &iss, bool &isstopped)
{
    Storage using_file;
    if(::signin(iss, using_file) == 0)
        user::shell(using_file);
}

void schedule::signup(std::istringstream &iss, bool &isstopped)
{
    Storage using_file;
    if(::signup(iss, using_file) == 0)
        user::shell(using_file);
}

//user::shell
void user::help(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    if(iss.eof())
        std::cout << HELP_USER << std::endl;
    else
        ::invalidCommand(iss);
}

void user::signout(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    if(::signout(iss, using_file) == 0)
        isstopped = true;
}

void user::editname(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::editname(iss, using_file);
}

void user::editpwd(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::editpwd(iss, using_file);
}

void user::cancel(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    if(::cancel(iss, using_file) == 0)
        isstopped = true;
}

void user::addtask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::addtask(iss, using_tasks);
}

void user::edittask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::edittask(iss, using_tasks);
}

void user::showtask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::showtask(iss, using_tasks);
}

void user::deltask(std::istringstream &iss, bool &isstopped, Storage &using_file, Tasks &using_tasks)
{
    ::deltask(iss, using_tasks);
}

//terminal
int terminal::help()
{
    std::cout << HELP_TERMINAL << std::endl;
    return 0;
}

int terminal::invalidCommand(int argc, char *argv [])
{
    std::cerr << "Invalid command:";
    for(int i = 1; i < argc; ++i)
        std::cerr << " " << argv[i];
    std::cerr << "\ntry help" << std::endl;
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

int terminal::signin(const char *user_name, const char *password, Storage &using_file)
{
    using_file.signin(user_name);
    if(using_file.fail()) return 1;
    if(!using_file.user().verify_password(password))
    {
        using_file.signout();
        std::cerr << "signin: Wrong password" << std::endl;
        return 1;
    }
    return 0;
}

int terminal::editname(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::editname(iss, using_file);
}

int terminal::editpwd(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::editpwd(iss, using_file);
}

int terminal::cancel(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::cancel(iss, using_file);
}

int terminal::addtask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::addtask(iss, using_tasks);
}

int terminal::edittask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::edittask(iss, using_tasks);
}

int terminal::showtask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::showtask(iss, using_tasks);
}

int terminal::deltask(std::istringstream &iss, Storage &using_file, Tasks &using_tasks)
{
    return ::deltask(iss, using_tasks);
}
