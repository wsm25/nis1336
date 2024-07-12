#include<iostream>
#include"storage.h"
#include"tasks.h"

void addtask()
{
   
}

void deltask()
{

}

void showtask()
{

}

//第一次打开文件的操作
bool Open_By_Username(const char* user_name,const char* your_password,Storage &using_file)
{
    User user;
    user = using_file.user();
    if(using_file.fail())
    {
        if(user.set_password(your_password))
        {
            std::cout << "Create your account successfully" << std::endl;
            return true;
        }
        std::cout << "Invalid password" << std::endl;
        return false;
    }

    else if(!user.verify_password(your_password))
    {
        std::cout << "Your password is wrong" << std::endl;
        return false;
    }

    else if(user.verify_password(your_password))
    {
        std::cout << "Open your account successfully!" << std::endl;
        return true;
    }

    Tasks tasks(using_file);
}