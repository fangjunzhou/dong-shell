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
    m_charBuffer = new DisplayBuffer<char>(size, '\0');
    m_flushBitmap = new DisplayBuffer<bool>(size, true);
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
    std::cout << CLEAR_SCR;
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
            std::cout << MOVE_CURSOR(x, y);
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

DisplayBuffer<char> &VerticalDisplay::GetCharBuffer()
{
    return *m_charBuffer;
}

DisplayBuffer<bool> &VerticalDisplay::GetFlushBitmap()
{
    return *m_flushBitmap;
}

#pragma endregion
