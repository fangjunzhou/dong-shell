#include <iostream>
#include <fstream>
#include <csignal>

#include <ext/stdio_filebuf.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "callbacklist.h"

#include "config.h"
#include "CommandHandler.hpp"
#include "VerticalDisplay.hpp"

struct winsize winSize;
eventpp::CallbackList<void(int width, int height)> terminalResizeCallback;

// The PID of the display process.
pid_t displayPid;

// The input fd to send clear command to vertical output.
int *clearPipeFdWriteEnd;

void SIGWINCH_Handler(int sigNum)
{
    if (sigNum == SIGWINCH)
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize);

        // Call the resize callback.
        terminalResizeCallback(winSize.ws_col, winSize.ws_row);
    }
}

struct ClearCommandArgv
{
    int clearPipeReadFd;
    VerticalDisplay *verticalDisplay;
};

void *ClearCommandHandle(void *argv)
{
    ClearCommandArgv *clearCommandArgv = (ClearCommandArgv *)argv;
    // Get the read end of the pipe, and VerticalDisplay obj.
    int clearPipeFdReadEnd = clearCommandArgv->clearPipeReadFd;
    VerticalDisplay *verticalDisplay = clearCommandArgv->verticalDisplay;

    // Clear pipe buffer.
    int clearBuffer;

    while (true)
    {
        // Check the clear command pipe.
        if (read(clearPipeFdReadEnd, &clearBuffer, 1) != 0)
        {
            verticalDisplay->ClearDisplay();
        }
    }
}

int main(int, char **)
{
    // Getting the console width and height.
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize);

    // Register SIGWINCH signal
    signal(SIGWINCH, SIGWINCH_Handler);

    // Init VerticleDisplay
    VerticalDisplay display = VerticalDisplay(&terminalResizeCallback, {winSize.ws_col, winSize.ws_row}, 2);

    // Create vertical output pipe.
    int outputPipeFd[2];
    pipe(outputPipeFd);

    // Create pipe for clear operation.
    int clearPipeFd[2];
    clearPipeFdWriteEnd = clearPipeFd + 1;
    pipe(clearPipeFd);

    displayPid = fork();
    if (displayPid == 0)
    {
        // Display Process
        std::cout << "\033[2J \033[H";
        display.ClearDisplay();
        // Redirect the stdin
        dup2(outputPipeFd[0], STDIN_FILENO);

        // Start the clear handle thread.
        pthread_t clearThread;

        ClearCommandArgv argv = {clearPipeFd[0], &display};
        pthread_create(&clearThread, NULL, ClearCommandHandle, &argv);

        while (true)
        {
            display.PushChar((char)std::cin.get());
            display.Flush();
        }
    }
    else
    {
        // Redirect the stdout
        dup2(outputPipeFd[1], STDOUT_FILENO);
    }

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
        int res = CommandHandler::HandleCommand(command, *clearPipeFdWriteEnd, displayPid);
        if (res == -1)
        {
            std::cerr << "CommandHandler error\n";
            exit(1);
        }
        else if (res == 1)
        {
            continue;
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