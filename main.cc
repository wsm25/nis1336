#include "Command.h"
#include <iostream>

int main(int argc, char *argv [])
{
    //help
    if(argc == 2 && !strcmp(argv[1], "help"))
    {
        std::cout << HELP_TERMINAL << std::endl;
        return 0;
    }
    //run in shell
    if(argc == 2 && !strcmp(argv[1], "run"))
    {
        shell();
        return 0;
    }
    //run in terminal
    if(argc > 4)
    {
        char *using_user = argv[1];
        char *input_password = argv[2];
        Storage opening_file(using_user);
        if(!Open_By_Username(using_user, input_password, opening_file))
            return 0;
        char *command = argv[3];
        ///TODO：command we want
        std::string inputLine;
        for(int i = 4; i < argc; ++i)
            inputLine += std::string(argv[i]);
        std::istringstream iss(inputLine);
        bool flag;
        cmds.at(command)(iss, flag, opening_file);

        return 0;
    }
    std::cerr << argv[0] << ": invalid command" << std::endl;
    return 1;
}
