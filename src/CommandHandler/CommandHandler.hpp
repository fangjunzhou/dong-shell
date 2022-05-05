#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace CommandHandler
{
    /**
     * @brief Process the reserved command.
     *
     * @param command the string command pass to the handler.
     * @return int -1 if fail, 0 if success.
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
}

#endif // __COMMANDHANDLER_H__