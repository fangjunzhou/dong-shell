#include <catch2/catch_test_macros.hpp>
#include "ShellDisplay/ShellDisplay.hpp"

TEST_CASE("DisplayBuffer Constructor and Destructor", "[DisplayBuffer]")
{
    DisplayBuffer<int> buffer = DisplayBuffer<int>({10, 10});
}