#ifndef __VERTICALDISPLAY_H__
#define __VERTICALDISPLAY_H__
#include <iostream>

#include "DisplayBuffer.hpp"
#include "eventpp/callbacklist.h"

using ResizeCL = eventpp::CallbackList<void(int width, int height)>;

class VerticalDisplay
{
private:
    /**
     * @brief The buffer that stores all the character;
     *
     */
    DisplayBuffer<char> *m_charBuffer;
    /**
     * @brief The buffer that stores which position needs flush.
     * Clear all bits to 0 when flushing.
     *
     */
    DisplayBuffer<bool> *m_flushBitmap;

    /**
     * @brief The console resize event passed in to the constructor.
     *
     */
    ResizeCL *m_consoleResizeEvent;

    /**
     * @brief Console resize callback handle.
     * Use this to unregister callback.
     *
     */
    ResizeCL::Handle m_consoleResizeHandle;

    /**
     * @brief Console resize callback function.
     *
     * @param width the new console width.
     * @param height the new console height.
     */
    void OnConsoleResize(int width, int height);

public:
    VerticalDisplay(eventpp::CallbackList<void(int width, int height)> *consoleResizeCallback, BufferSize size);
    ~VerticalDisplay();
};

#endif // __VERTICALDISPLAY_H__