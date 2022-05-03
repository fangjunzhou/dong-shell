#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "config.h"

int main(int, char **)
{
    // Debug purpose: Close cout and open a file.
    std::ofstream redirFile;
    int stdoutFileNo = dup(STDOUT_FILENO);

    close(STDOUT_FILENO);
    redirFile.open("redir.txt");

    std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;
    std::string command;

    redirFile.close();

    dup2(stdoutFileNo, STDOUT_FILENO);

    // Main loop.
    while (true)
    {
        std::cout << "Dong Shell>";
        std::cin >> command;
        std::cout << "Command: " << command << std::endl;
    }
}