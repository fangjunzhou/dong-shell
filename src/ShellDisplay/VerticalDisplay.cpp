#include "VerticalDisplay.hpp"

#define CLEAR_SCR "\033[2J"
#define MOVE_CURSOR(x, y) ("\033[" + std::to_string(y) + ";" + std::to_string(x) + "H")

#pragma region Constructor and Destructor

VerticalDisplay::VerticalDisplay(eventpp::CallbackList<void(int width, int height)> *consoleResizeCallback, BufferSize size)
{
    // Register callback
    m_consoleResizeEvent = consoleResizeCallback;
    if (m_consoleResizeEvent != NULL)
    {
        m_consoleResizeHandle = m_consoleResizeEvent->append([this](int width, int height)
                                                             { OnConsoleResize(width, height); });
    }

    m_consoleSize = size;

    // Init DisplayBuffer
    m_charBuffer = new DisplayBuffer<char>(size, ' ');
    m_flushBitmap = new DisplayBuffer<bool>(size, true);

    // Set the virtual cursor to (width - 1, 0)
    m_virtualCursorPos = {size.width - 1, 0};

    // Set history buffer head.
    m_bufferHead = m_historyBuffer;
    m_bufferTail = m_historyBuffer;

    // Clear the history buffer.
    for (int i = 0; i < k_historyBufferSize; i++)
    {
        m_historyBuffer[i] = '\0';
    }
    m_historyLines = 0;
}

VerticalDisplay::~VerticalDisplay()
{
    // Unregister callback;
    if (m_consoleResizeEvent != NULL)
    {
        m_consoleResizeEvent->remove(m_consoleResizeHandle);
    }

    // Deallocate DisplayBuffer
    delete m_charBuffer;
    delete m_flushBitmap;
}

#pragma endregion

#pragma region Private Field

void VerticalDisplay::OnConsoleResize(int width, int height)
{
    // Reset the console size.
    m_consoleSize.width = width;
    m_consoleSize.height = height;
}

#pragma endregion

#pragma region Public Field

void VerticalDisplay::ClearDisplay()
{
    // Clear all the buffer to initial state.
    for (int y = 0; y < m_consoleSize.height; y++)
    {
        for (int x = 0; x < m_consoleSize.width; x++)
        {
            UpdateChar(x, y, ' ');
        }
    }
    Flush();
}

void VerticalDisplay::Flush()
{
    // Flush all the content in the buffer onto the screen.
    for (int y = 0; y < m_consoleSize.height; y++)
    {
        for (int x = 0; x < m_consoleSize.width; x++)
        {
            // Check bitmap.
            if (!m_flushBitmap->GetBuffer()[x][y])
                continue;
            // Move cursor.
            std::cout << MOVE_CURSOR(x, y + 1);
            std::cout << m_charBuffer->GetBuffer()[x][y];
            // Make bitmap cold.
            m_flushBitmap->SetBuffer(x, y, false);
        }
    }
}

void VerticalDisplay::UpdateChar(int x, int y, char val)
{
    m_charBuffer->SetBuffer(x, y, val);
    m_flushBitmap->SetBuffer(x, y, true);
}

void VerticalDisplay::PushString(std::string str)
{
    // The number of char in current line.
    int lineCharCount = 0;
    for (int i = 0; i < str.length(); i++)
    {
        // Count history lines.
        if (*m_bufferTail == '\n')
            m_historyLines--;
        // Start a new line.
        if (str[i] == '\n' || lineCharCount == m_consoleSize.height)
        {
            m_historyLines++;
            m_displayLines++;
            lineCharCount = 0;
        }
        // Push the string to the circular buffer and increase the pointer.
        *m_bufferTail = str[i];
        m_bufferTail = m_historyBuffer + (m_bufferTail - m_historyBuffer + 1) % k_historyBufferSize;

        lineCharCount++;

        // Push the buffer head.
        if (m_bufferTail == m_bufferHead)
            m_bufferHead++;
        if (m_bufferTail == m_displayHead)
            m_displayHead++;
    }

    // TODO: Append new chars to the buffer to optimize performance.
    if (m_displayLines <= m_consoleSize.width)
    {
        lineCharCount = 0;
        for (int i = 0; i < str.length(); i++)
        {
            // Start a new line.
            if (str[i] == '\n' || lineCharCount == m_consoleSize.height)
            {
                m_virtualCursorPos.x--;
                m_virtualCursorPos.y = 0;
                lineCharCount = 0;
            }
            if (str[i] == '\n')
                continue;
            // Update the char.
            UpdateChar(m_virtualCursorPos.x, m_virtualCursorPos.y, str[i]);
            m_virtualCursorPos.y++;
            lineCharCount++;
        }
        return;
    }

    // The max available display lines should = console width.
    lineCharCount = 1;
    while (m_displayLines > m_consoleSize.width)
    {
        while (*m_displayHead != '\n' && lineCharCount != m_consoleSize.height)
        {
            m_displayHead = m_historyBuffer + (m_displayHead - m_historyBuffer + 1) % k_historyBufferSize;
            lineCharCount++;
        }
        m_displayHead = m_historyBuffer + (m_displayHead - m_historyBuffer + 1) % k_historyBufferSize;
        m_displayLines--;
        lineCharCount = 1;
    }

    // TODO: Refresh the entire buffer.
    ClearDisplay();
    m_virtualCursorPos = {m_consoleSize.width - 1, 0};

    lineCharCount = 0;
    // Traverse through displayHead to bufferTail.
    for (char *ptr = m_displayHead; ptr != m_bufferTail; ptr = m_historyBuffer + (ptr - m_historyBuffer + 1) % k_historyBufferSize)
    {
        // Start a new line.
        if (*ptr == '\n' || lineCharCount == m_consoleSize.height)
        {
            m_virtualCursorPos.x--;
            m_virtualCursorPos.y = 0;
            lineCharCount = 0;
            continue;
        }
        // Update the char.
        UpdateChar(m_virtualCursorPos.x, m_virtualCursorPos.y, *ptr);
        m_virtualCursorPos.y++;
        lineCharCount++;
    }
}

DisplayBuffer<char> &VerticalDisplay::GetCharBuffer()
{
    return *m_charBuffer;
}

DisplayBuffer<bool> &VerticalDisplay::GetFlushBitmap()
{
    return *m_flushBitmap;
}

#pragma endregion
