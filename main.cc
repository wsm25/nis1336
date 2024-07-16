#include "shell.h"
#include <iostream>
#include <sstream>

int main(int argc, char *argv [])
{
    //help
    if(argc == 2 && !strcmp(argv[1], "help"))
        return terminal::help();

    //run in shell
    if(argc == 2 && !strcmp(argv[1], "run"))
        return schedule::shell();
    
    //run in terminal
    //signup
    if(argc == 4 && !strcmp(argv[3], "signup"))
        return terminal::signup(argv[1], argv[2]);
    
    //other command
    if(argc >= 4)
    {
        //signin
        Storage storage;
        if(terminal::signin(argv[1], argv[2], storage)) return 1;
        Tasks tasks(storage);

        std::string command = argv[3];
        std::string inputLine;
        for(int i = 4; i < argc; ++i)
        {
            inputLine += ' ';
            inputLine += argv[i];
        }
        std::istringstream iss(inputLine);

        terminal::CMDS::const_iterator it = terminal::cmds.find(command);
        if(it != terminal::cmds.end())
            return it->second(iss, storage, tasks);
    }
    //error
    terminal::invalidCommand(argc, argv);
}
