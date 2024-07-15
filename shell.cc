#include "Command.h"

void shell()
{
    std::cout << "Please enter your username: " << std::endl;
    std::string user_name, your_password;
    std::cin >> user_name;
    std::cout << "Enter your password: " << std::endl;
    std::cin >> your_password;
    Storage using_file(user_name.c_str());
    if(!Open_By_Username(user_name.c_str(), your_password.c_str(), using_file))
        return;
    Tasks tasks(using_file);

    ///创建输入线程&&定时提示线程
    pthread_t remind_thread;

    ///取消cin与cout的同步
    std::ios::sync_with_stdio(false);

    if(pthread_create(&remind_thread, NULL, Remind, (void *)&tasks)) {
        std::cerr << "Fail to creat WARNING_THREAD" << std::endl;
        return;
    }

    bool flag = true;

    while(flag)
    {
        std::cout << "(schedule) ";
        std::string inputLine;
        getline(std::cin, inputLine);
        std::istringstream iss(inputLine);
        std::string command;
        iss >> command;
        cmds.at(command)(iss, flag, using_file);

    }
}
