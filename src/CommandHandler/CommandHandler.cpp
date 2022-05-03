#include "CommandHandler.hpp"

int CommandHandler::HandleCommand(std::string &command)
{
    // Exit the process
    if (command == "exit")
    {
        exit(0);
    }

    return 0;
}
