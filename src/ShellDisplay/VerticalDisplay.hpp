#ifndef __VERTICALDISPLAY_H__
#define __VERTICALDISPLAY_H__
#include <iostream>
#include <string>

#include "DisplayBuffer.hpp"
#include "eventpp/callbacklist.h"

using ResizeCL = eventpp::CallbackList<void(int width, int height)>;

class VerticalDisplay
{
private:
    /**
     * @brief The current size of console.
     *
     */
    BufferSize m_consoleSize;

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

    /**
     * @brief Clear the entire display.
     *
     */
    void ClearDisplay();

    /**
     * @brief Flush the m_charBuffer onto the screen.
     *
     */
    void Flush();

    /**
     * @brief Update the char for a coordinate.
     *
     * @param x x coord.
     * @param y y coord.
     * @param val the char value.
     */
    void UpdateChar(int x, int y, char val);

    /**
     * @brief Get the Char Buffer directly.
     *
     * @return DisplayBuffer<char>&
     */
    DisplayBuffer<char> &GetCharBuffer();

    /**
     * @brief Get the Flush Bitmap directly.
     *
     * @return DisplayBuffer<bool>&
     */
    DisplayBuffer<bool> &GetFlushBitmap();
};

#endif // __VERTICALDISPLAY_H__