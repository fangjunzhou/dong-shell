#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <unistd.h>
#include "callbacklist.h"

class CommandHandler
{
private:
    /**
     * @brief The read file descriptor for clear buffer pipe.
     *
     */
    int m_clearBufferFd;

    /**
     * @brief The PID for display process.
     *
     */
    int m_displayPid;

    eventpp::CallbackList<void()> *m_applicationQuitCallback;

public:
    /**
     * @brief Construct a new Command Handler object
     *
     * @param clearBufferFd The read fd for clear pipe.
     * @param displayPid the PID for display process.
     * @param applicationQuitCallback callback list to call when exiting the application.
     */
    CommandHandler(int clearBufferFd, int displayPid, eventpp::CallbackList<void()> *applicationQuitCallback);

    /**
     * @brief Process the reserved command.
     *
     * @param command the string command pass to the handler.
     * @return int -1 if fail, 0 if no action, 1 if success.
     */
    int HandleCommand(const std::string &command);

    /**
     * @brief Slip the command based on space and tabs.
     * Allocating a new vector of strings on heap and callers are responsable to deallocate them.
     *
     * @param command the string command copy.
     * @return std::vector<std::string> & an vector of strings.
     */
    std::vector<std::string> &SplitCommand(std::string command);

    /**
     * @brief Debug the command list info.
     *
     * @param command the original command in std::string.
     * @param commandList the command list vector.
     * @param argv the converted char *const *argv array.
     * @param argc the argument count.
     */
    void DebugCommandList(std::string *command, std::vector<std::string> *commandList,
                          char *const *argv, int argc);
};

#endif // __COMMANDHANDLER_H__