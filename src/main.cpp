#include <iostream>
#include <fstream>
#include <csignal>

#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "config.h"
#include "callbacklist.h"
#include "CommandHandler.hpp"
#include "DisplayBuffer.hpp"

struct winsize winSize;
eventpp::CallbackList<void(int width, int height)> terminalResizeCallback;

void SIGWINCH_Handler(int sigNum)
{
    if (sigNum == SIGWINCH)
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize);

        // Call the resize callback.
        terminalResizeCallback(winSize.ws_col, winSize.ws_row);
    }
}

int main(int, char **)
{
    // Print version.
    std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;

    // Getting the console width and height.
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize);
    std::cout << "Width: " << winSize.ws_col << std::endl;
    std::cout << "Height: " << winSize.ws_row << std::endl;

    // Register SIGWINCH signal
    signal(SIGWINCH, SIGWINCH_Handler);

    // Init display buffer
    DisplayBuffer<int> buffer = DisplayBuffer<int>({10, 10});

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
            char **argv = new char *[commandList.size() + 1];
            for (int i = 0; i < commandList.size(); i++)
            {
                argv[i] = (char *const)commandList[i].c_str();
            }
            argv[commandList.size()] = NULL;

            CommandHandler::DebugCommandList(&command, &commandList, argv, commandList.size());

            if (commandList.size() > 0)
            {
                if (execv(commandList[0].c_str(), argv) == -1)
                {
                    std::cout << "Dong Shell: execv error.\n";
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