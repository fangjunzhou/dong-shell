#include <iostream>
#include <fstream>

#include <string>

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
        std::cout << "Dong Shell>";
        std::cin >> command;
        std::cout << "Command: " << command << std::endl;
        int res = CommandHandler::HandleCommand(command);
        if (res == -1)
        {
            std::cerr << "CommandHandler error\n";
            exit(1);
        }

        // Fork a new process and execute the target execuatble.
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child Process
            std::cout << "Child process executing: " << command << std::endl;

            // TODO: Execute here.

            exit(0);
        }
        // Wait for child process finished.
        waitpid(pid, NULL, 0);
        std::cout << "Forked child process " << pid << std::endl;
    }
}