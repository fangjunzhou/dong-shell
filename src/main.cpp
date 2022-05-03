#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "CommandHandler/CommandHandler.hpp"

int main(int, char **)
{
    // Print version.
    std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;

    std::string command;

    // Main loop.
    while (true)
    {
        command.clear();
        std::cout << "Dong Shell>";
        std::getline(std::cin, command);
        std::cout << "Command: " << command << std::endl;
        int res = CommandHandler::HandleCommand(command);
        if (res == -1)
        {
            std::cerr << "CommandHandler error\n";
            exit(1);
        }

        // Split the command.
        std::vector<std::string> &commandList = CommandHandler::SplitCommand(command);

        // Fork a new process and execute the target execuatble.
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child Process
            std::cout << "Child process executing: " << command << std::endl;

            // TODO: Remove this debug print.
            std::cout << "commandList: \n";
            for (int i = 0; i < commandList.size(); i++)
            {
                std::cout << commandList[i];
                if (i != commandList.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;

            // TODO: Construct the arguments and path.

            exit(0);
        }
        // Wait for child process finished.
        waitpid(pid, NULL, 0);
        std::cout << "Forked child process " << pid << std::endl;

        // Deallocating the vector
        delete &commandList;
    }
}