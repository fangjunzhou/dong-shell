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
        std::cout << "Dong Shell> ";
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
            // Construct the arguments and path.
            // Convert the std::vector<std::string> to char *[] on heap.
            char **argv = new char *[commandList.size()];
            for (int i = 0; i < commandList.size(); i++)
            {
                argv[i] = (char *const)commandList[i].c_str();
            }

            if (commandList.size() > 0)
            {
                if (execv(commandList[0].c_str(), argv) == -1)
                {
                    std::cout << "Dong Shell: execv error, no executable named "
                              << commandList[0].c_str() << std::endl;
                }
            }

            exit(0);
        }
        // Wait for child process finished.
        waitpid(pid, NULL, 0);

        // Deallocating the vector
        delete &commandList;
    }
}