#ifndef __SHELLDISPLAY_H__
#define __SHELLDISPLAY_H__
#include <stdexcept>

struct BufferSize
{
    int width;
    int height;
};

/**
 * @brief The buffer base class to store screen characters.
 *
 */
template <class T>
class DisplayBuffer
{
private:
    BufferSize m_size;
    T **m_buffer;
    /**
     * @brief Deallocate all the content in the buffer.
     *
     */
    void DeallocateBuffer()
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

    /**
     * @brief Allocate new buffer.
     *
     * @param width the width of the buffer.
     * @param height the height of the buffer.
     */
    void AllocateBuffer(int width, int height)
    {
        // Create the buffer.
        m_buffer = new T *[width];
        for (int i = 0; i < width; i++)
        {
            m_buffer[i] = new T[height];
        }
    }

public:
    /**
     * @brief Construct a new Display Buffer object
     *
     * @param size
     * @throws std::invalid_argument when width or height is less than 0.
     */
    DisplayBuffer(BufferSize size)
    {
        if (size.width < 0 || size.height < 0)
        {
            throw std::invalid_argument("Width or height less than 0.");
        }
        m_size = size;
        AllocateBuffer(size.width, size.height);
    }
    ~DisplayBuffer()
    {
        DeallocateBuffer();
    }
    /**
     * @brief Set the size of the buffer.
     *
     * @param size the new buffer size.
     * @throws std::invalid_argument when width or height is less than 0.
     */
    void SetBufferSize(BufferSize size)
    {
        if (size.width < 0 || size.height < 0)
        {
            throw std::invalid_argument("Width or height less than 0.");
        }
        m_size = size;
        DeallocateBuffer();
        AllocateBuffer(size.width, size.height);
    }
    /**
     * @brief Get the current size of buffer.
     *
     * @return BufferSize current buffer size.
     */
    BufferSize GetBufferSize()
    {
        return m_size;
    }

    /**
     * @brief Set the buffer content.
     *
     * @param x the x coordinate.
     * @param y the y coordinate.
     * @param value the value to set.
     * @throws std::out_of_range when x or y is out of range.
     */
    void SetBuffer(int x, int y, T value)
    {
        if (x < 0 || x >= m_size.width || y < 0 || y >= m_size.height)
        {
            throw std::out_of_range("x or y coordinate out of range");
        }
        m_buffer[x][y] = value;
    }

    /**
     * @brief Read the value in the buffer.
     *
     * @param x the x cooridnate of the buffer.
     * @param y the y coordinate of the buffer.
     * @return T the value in the location.
     * @throws std::out_of_range when x or y is out of range.
     */
    T ReadBuffer(int x, int y)
    {
        if (x < 0 || x >= m_size.width || y < 0 || y >= m_size.height)
        {
            throw std::out_of_range("x or y coordinate out of range");
        }
        return m_buffer[x][y];
    }
};

#endif // __SHELLDISPLAY_H__