#include "shell.h"

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
        return terminal::shell(argc, argv);
    //error
    return terminal::invalidCommand(argc, argv);
}
