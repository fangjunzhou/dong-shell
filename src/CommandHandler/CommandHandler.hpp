#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__
#include <string>
#include <vector>
#include <algorithm>

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
}

#endif // __COMMANDHANDLER_H__