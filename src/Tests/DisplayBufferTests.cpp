#include <catch2/catch_test_macros.hpp>
#include "DisplayBuffer.hpp"

TEST_CASE("DisplayBuffer Constructor and Destructor", "[DisplayBuffer]")
{
    SECTION("Stack Allocated DisplayBuffer")
    {
        DisplayBuffer<int> buffer = DisplayBuffer<int>({2, 4});

        BufferSize size = buffer.GetBufferSize();
        REQUIRE(size.width == 2);
        REQUIRE(size.height == 4);
    }

    SECTION("Heap Allocated DisplayBuffer")
    {
        DisplayBuffer<int> *buffer = new DisplayBuffer<int>({2, 4});

        BufferSize size = buffer->GetBufferSize();
        REQUIRE(size.width == 2);
        REQUIRE(size.height == 4);

        delete buffer;
        SUCCEED("Deallocated buffer obj.");
    }

    SECTION("Constructor Invalid Arguments")
    {
        try
        {
            DisplayBuffer<int> buffer = DisplayBuffer<int>({-1, 4});
            FAIL("std::invalid_argument not thrown.");
        }
        catch (const std::invalid_argument &IAE)
        {
            SUCCEED("Successfully throw exception.");
        }
    }
}

TEST_CASE("Display Buffer Update Buffer Size", "[DisplayBuffer]")
{
    DisplayBuffer<int> buffer = DisplayBuffer<int>({2, 4});
    SECTION("Get Buffer Size")
    {
        BufferSize size = buffer.GetBufferSize();
        REQUIRE(size.width == 2);
        REQUIRE(size.height == 4);
    }

    SECTION("Buffer Resize")
    {
        buffer.SetBufferSize({8, 16});
        BufferSize size = buffer.GetBufferSize();
        REQUIRE(size.width == 8);
        REQUIRE(size.height == 16);
    }
}

TEST_CASE("Buffer Read and Write", "[DisplayBuffer]")
{
    DisplayBuffer<int> buffer = DisplayBuffer<int>({2, 4});

    buffer.SetBuffer(0, 1, 128);
    REQUIRE(buffer.ReadBuffer(0, 1) == 128);
    REQUIRE(buffer.GetBuffer()[0][1] == 128);
}
