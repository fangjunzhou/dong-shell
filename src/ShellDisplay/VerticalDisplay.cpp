#include "VerticalDisplay.hpp"

#pragma region Constructor and Destructor

VerticalDisplay::VerticalDisplay(eventpp::CallbackList<void(int width, int height)> *consoleResizeCallback, BufferSize size)
{
    // Register callback
    m_consoleResizeEvent = consoleResizeCallback;
    m_consoleResizeHandle = m_consoleResizeEvent->append([this](int width, int height)
                                                         { OnConsoleResize(width, height); });

    // Init DisplayBuffer
    m_charBuffer = new DisplayBuffer<char>(size);
    m_flushBitmap = new DisplayBuffer<bool>(size);
}

VerticalDisplay::~VerticalDisplay()
{
    // Unregister callback;
    m_consoleResizeEvent->remove(m_consoleResizeHandle);

    // Deallocate DisplayBuffer
    delete m_charBuffer;
    delete m_flushBitmap;
}

#pragma endregion

void VerticalDisplay::OnConsoleResize(int width, int height)
{
}
