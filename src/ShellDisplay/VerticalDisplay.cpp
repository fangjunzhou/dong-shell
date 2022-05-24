#include "VerticalDisplay.hpp"

#define CLEAR_SCR "\033[2J"
#define MOVE_CURSOR(x, y) ("\033[" + std::to_string(y) + ";" + std::to_string(x) + "H")

#pragma region Constructor and Destructor

VerticalDisplay::VerticalDisplay(eventpp::CallbackList<void(int width, int height)> *consoleResizeCallback,
                                 BufferSize size,
                                 int lineSpace)
{
    // Register callback
    m_consoleResizeEvent = consoleResizeCallback;
    if (m_consoleResizeEvent != NULL)
    {
        m_consoleResizeHandle = m_consoleResizeEvent->append([this](int width, int height)
                                                             { OnConsoleResize(width, height); });
    }

    m_lineSpace = lineSpace;
    m_consoleSize = {size.width / lineSpace, size.height};

    // Init DisplayBuffer
    m_charBuffer = new DisplayBuffer<char>(m_consoleSize, ' ');
    m_flushBitmap = new DisplayBuffer<bool>(m_consoleSize, true);

    // Set the virtual cursor to (width - 1, 0)
    m_virtualCursorPos = {m_consoleSize.width - 1, 0};

    // Set history buffer head.
    m_bufferHead = m_historyBuffer;
    m_displayHead = m_historyBuffer;
    m_bufferTail = m_historyBuffer;

    // Clear the history buffer.
    for (int i = 0; i < k_historyBufferSize; i++)
    {
        m_historyBuffer[i] = '\0';
    }
    m_historyLines = 0;
    m_displayLines = 1;
    m_lineCharCount = 0;
}

VerticalDisplay::VerticalDisplay(eventpp::CallbackList<void(int width, int height)> *consoleResizeCallback,
                                 BufferSize size) : VerticalDisplay(consoleResizeCallback,
                                                                    size,
                                                                    1)
{
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
    m_consoleSize = {width / m_lineSpace, height};

    // Resize the buffer
    m_charBuffer->SetBufferSize(m_consoleSize);
    m_flushBitmap->SetBufferSize(m_consoleSize);

    // Set the virtual cursor to (width - 1, 0)
    m_virtualCursorPos = {m_consoleSize.width - 1, 0};

    // Clear the entire display and push all the history again.
    ClearDisplay();
    std::string historyStr;
    char *curr = m_bufferHead;
    while (curr != m_bufferTail)
    {
        historyStr += *curr;
        curr++;
    }
    PushString(historyStr);
    Flush();
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
    // Clear the entire display
    std::cout << CLEAR_SCR;

    // Move cursor.
    m_virtualCursorPos.x = m_consoleSize.width - 1;
    m_virtualCursorPos.y = 0;

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
            std::cout << MOVE_CURSOR(x * m_lineSpace + 1, y + 1);
            std::cout << m_charBuffer->GetBuffer()[x][y];
            // Make bitmap cold.
            m_flushBitmap->SetBuffer(x, y, false);
        }
    }
    // Move to cursor position
    std::cout << MOVE_CURSOR(m_virtualCursorPos.x * m_lineSpace + 1, m_virtualCursorPos.y + 1);
}

void VerticalDisplay::UpdateChar(int x, int y, char val)
{
    m_charBuffer->SetBuffer(x, y, val);
    m_flushBitmap->SetBuffer(x, y, true);
}

void VerticalDisplay::PushString(std::string str)
{
    // The number of char in current line.
    int initLineCharCount = m_lineCharCount;
    int lineCharCount = m_lineCharCount;
    for (int i = 0; i < str.length(); i++)
    {
        // Count history lines.
        if (*m_bufferTail == '\n')
            m_historyLines--;

        lineCharCount++;

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

        // Push the buffer head.
        if (m_bufferTail == m_bufferHead)
            m_bufferHead = m_historyBuffer + (m_bufferHead - m_historyBuffer + 1) % k_historyBufferSize;
        if (m_bufferTail == m_displayHead)
            m_displayHead = m_historyBuffer + (m_displayHead - m_historyBuffer + 1) % k_historyBufferSize;
    }
    m_lineCharCount = lineCharCount;

    // Append new chars to the buffer to optimize performance.
    if (m_displayLines <= m_consoleSize.width)
    {
        lineCharCount = initLineCharCount;
        for (int i = 0; i < str.length(); i++)
        {
            lineCharCount++;
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

    // Refresh the entire buffer.
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

void VerticalDisplay::PushChar(char ch)
{
    if (ch == '\0')
        return;

    // The number of char in current line.
    int initLineCharCount = m_lineCharCount;
    int lineCharCount = m_lineCharCount;
    // Count history lines.
    if (*m_bufferTail == '\n')
        m_historyLines--;

    lineCharCount++;

    // Start a new line.
    if (ch == '\n' || lineCharCount >= m_consoleSize.height)
    {
        m_historyLines++;
        m_displayLines++;
        lineCharCount -= m_consoleSize.height;
    }
    // Push the string to the circular buffer and increase the pointer.
    *m_bufferTail = ch;
    m_bufferTail = m_historyBuffer + (m_bufferTail - m_historyBuffer + 1) % k_historyBufferSize;

    // Push the buffer head.
    if (m_bufferTail == m_bufferHead)
        m_bufferHead = m_historyBuffer + (m_bufferHead - m_historyBuffer + 1) % k_historyBufferSize;
    if (m_bufferTail == m_displayHead)
        m_displayHead = m_historyBuffer + (m_displayHead - m_historyBuffer + 1) % k_historyBufferSize;
    m_lineCharCount = lineCharCount;

    // Append new chars to the buffer to optimize performance.
    if (m_displayLines <= m_consoleSize.width)
    {
        lineCharCount = initLineCharCount + 1;
        // Start a new line.
        if (ch == '\n' || lineCharCount == m_consoleSize.height)
        {
            m_virtualCursorPos.x--;
            m_virtualCursorPos.y = 0;
            lineCharCount = 0;
        }
        if (ch == '\n')
            return;
        // Update the char.
        UpdateChar(m_virtualCursorPos.x, m_virtualCursorPos.y, ch);
        m_virtualCursorPos.y++;
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

    // Refresh the entire buffer.
    ClearDisplay();
    m_virtualCursorPos = {m_consoleSize.width - 1, 0};

    lineCharCount = 1;
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
    m_lineCharCount = lineCharCount;
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
