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
#include <termios.h>

#include "callbacklist.h"

#include "config.h"
#include "CommandHandler.hpp"
#include "VerticalDisplay.hpp"

struct winsize winSize;
eventpp::CallbackList<void(int width, int height)> terminalResizeCallback;

// The PID of the IO process.
pid_t outputPid;
pid_t inputPid;

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

    int inputPipeFd[2];
    pipe(inputPipeFd);

    // Create pipe for clear operation.
    int clearPipeFd[2];
    clearPipeFdWriteEnd = clearPipeFd + 1;
    pipe(clearPipeFd);

    // Setup output process.
    outputPid = fork();
    if (outputPid == 0)
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

    // Setup input process.
    inputPid = fork();
    if (inputPid == 0)
    {
        // Input process.

        // Disable the terminal input display.
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        newt.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        char ch;
        while (true)
        {
            ch = std::cin.get();
            if (ch != '\0')
            {
                // Write the character to the vertical output.
                write(outputPipeFd[1], &ch, 1);
                // Send the character to the input pipe.
                write(inputPipeFd[1], &ch, 1);
            }
        }
    }
    else
    {
        // Redirect the stdin.
        dup2(inputPipeFd[0], STDIN_FILENO);
    }

    // Setup input process.

    // Print version.
    std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;

    std::string command;

    // TODO: Create CommandHandler obj.
    CommandHandler commandHandler = CommandHandler(*clearPipeFdWriteEnd, outputPid);

    // Main loop.
    while (true)
    {
        command.clear();
        std::cout << "Dong Shell> ";
        std::getline(std::cin, command);
        std::cout << "Command: " << command << std::endl;
        int res = commandHandler.HandleCommand(command);
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
        std::vector<std::string> &commandList = commandHandler.SplitCommand(command);

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

            commandHandler.DebugCommandList(&command, &commandList, argv, commandList.size());

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