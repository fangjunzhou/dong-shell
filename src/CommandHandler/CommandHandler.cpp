#include "CommandHandler.hpp"

int CommandHandler::HandleCommand(const std::string &command)
{
    // Exit the process
    if (command == "exit")
    {
        exit(0);
    }

    if (command == "clear")
    {
        std::cout << "\033[2J \033[H";
        return 1;
    }

    return 0;
}

std::vector<std::string> &CommandHandler::SplitCommand(std::string command)
{
    std::vector<std::string> *commandList = new std::vector<std::string>();

    // All the available delimiters.
    std::vector<std::string> delimiters = {" ", "\t"};
    // The current delimiter.
    std::string delimiter;
    // The start location of that delimiter.
    size_t delimiterLoc;
    // The splitted token;
    std::string token;

    // Split base on delimiters.
    while (true)
    {
        // Find the smallest delimiter and its position.
        delimiterLoc = std::string::npos;
        for (int i = 0; i < delimiters.size(); i++)
        {
            size_t newVal = command.find(delimiters[i]);
            if (newVal < delimiterLoc)
            {
                delimiter = delimiters[i];
                delimiterLoc = newVal;
            }
        }

        if (delimiterLoc == std::string::npos)
            break;

        token = command.substr(0, delimiterLoc);
        command.erase(0, delimiterLoc + delimiter.length());

        if (token.length() != 0)
        {
            commandList->push_back(token);
        }
    }

    if (command.length() != 0)
    {
        commandList->push_back(command);
    }

    return *commandList;
}

void CommandHandler::DebugCommandList(std::string *command, std::vector<std::string> *commandList,
                                      char *const *argv, int argc)
{
    if (command != NULL)
    {
        std::cout << "Original command: " << *command << std::endl;
    }

    if (commandList != NULL)
    {
        std::cout << "Command list vector: ";
        for (int i = 0; i < commandList->size(); i++)
        {
            std::cout << "\"" << (*commandList)[i] << "\"";
            if (i != commandList->size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    if (argv != NULL)
    {
        std::cout << "argv array: ";
        for (int i = 0; i < argc; i++)
        {
            std::cout << "\"" << argv[i] << "\"";
            if (i != argc - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}