#include "ShellDisplay.hpp"

#pragma region Constructor and Destructor

template <class T>
DisplayBuffer<T>::DisplayBuffer(BufferSize size)
{
    if (size.width < 0 || size.height < 0)
    {
        throw std::invalid_argument("Width or height less than 0.");
    }
    m_size = size;
    AllocateBuffer(size.width, size.height);
}

template <class T>
DisplayBuffer<T>::~DisplayBuffer()
{
    DeallocateBuffer();
}

#pragma endregion

#pragma region Private Methods

template <class T>
void DisplayBuffer<T>::DeallocateBuffer()
{
    if (m_buffer != NULL)
    {
        for (int i = 0; i < m_size.width; i++)
        {
            delete m_buffer[i];
        }
        delete m_buffer;
        m_buffer = NULL;
    }
}

template <class T>
void DisplayBuffer<T>::AllocateBuffer(int width, int height)
{
    // Create the buffer.
    m_buffer = new T *[width];
    for (int i = 0; i < width; i++)
    {
        m_buffer[i] = new T[height];
    }
}

#pragma endregion

#pragma region Public Methods

template <class T>
void DisplayBuffer<T>::SetBufferSize(BufferSize size)
{
    if (size.width < 0 || size.height < 0)
    {
        throw std::invalid_argument("Width or height less than 0.");
    }
    m_size = size;
    DeallocateBuffer();
    AllocateBuffer(size.width, size.height);
}

template <class T>
BufferSize DisplayBuffer<T>::GetBufferSize()
{
    return m_size;
}

template <class T>
void DisplayBuffer<T>::SetBuffer(int x, int y, T value)
{
    if (x < 0 || x >= m_size.width || y < 0 || y >= m_size.height)
    {
        throw std::out_of_range("x or y coordinate out of range");
    }
    m_buffer[x][y] = value;
}

template <class T>
T DisplayBuffer<T>::ReadBuffer(int x, int y)
{
    if (x < 0 || x >= m_size.width || y < 0 || y >= m_size.height)
    {
        throw std::out_of_range("x or y coordinate out of range");
    }
    return m_buffer[x][y];
}

#pragma endregion

#pragma region Template Instance

template class DisplayBuffer<int>;
template class DisplayBuffer<char>;

#pragma endregion