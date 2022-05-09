#ifndef __SHELLDISPLAY_H__
#define __SHELLDISPLAY_H__
#pragma once
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
    void DeallocateBuffer();

    /**
     * @brief Allocate new buffer.
     *
     * @param width the width of the buffer.
     * @param height the height of the buffer.
     */
    void AllocateBuffer(int width, int height);

public:
    /**
     * @brief Construct a new Display Buffer object
     *
     * @param size
     * @throws std::invalid_argument when width or height is less than 0.
     */
    DisplayBuffer(BufferSize size);

    ~DisplayBuffer();

    /**
     * @brief Set the size of the buffer.
     *
     * @param size the new buffer size.
     * @throws std::invalid_argument when width or height is less than 0.
     */
    void SetBufferSize(BufferSize size);

    /**
     * @brief Get the current size of buffer.
     *
     * @return BufferSize current buffer size.
     */
    BufferSize GetBufferSize();

    /**
     * @brief Set the buffer content.
     *
     * @param x the x coordinate.
     * @param y the y coordinate.
     * @param value the value to set.
     * @throws std::out_of_range when x or y is out of range.
     */
    void SetBuffer(int x, int y, T value);

    /**
     * @brief Get the buffer pointer directly.
     *
     * @return T** the pointer to buffer 2D array.
     */
    T **GetBuffer();

    /**
     * @brief Read the value in the buffer.
     *
     * @param x the x cooridnate of the buffer.
     * @param y the y coordinate of the buffer.
     * @return T the value in the location.
     * @throws std::out_of_range when x or y is out of range.
     */
    T ReadBuffer(int x, int y);
};

#include "DisplayBuffer.tpp"

#endif // __SHELLDISPLAY_H__