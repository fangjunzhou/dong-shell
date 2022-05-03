#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__
#include <string>

namespace CommandHandler
{
    /**
     * @brief Process the reserved command.
     *
     * @param command the string command pass to the handler.
     * @return int -1 if fail, 0 if success.
     */
    int HandleCommand(std::string &command);
}

#endif // __COMMANDHANDLER_H__