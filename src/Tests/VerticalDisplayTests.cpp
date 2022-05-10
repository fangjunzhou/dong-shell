#include <catch2/catch_test_macros.hpp>
#include "VerticalDisplay.hpp"

TEST_CASE("VerticalDisplay Flush Buffer", "[VerticalDisplay]")
{
    VerticalDisplay display = VerticalDisplay(NULL, {10, 10});
    display.ClearDisplay();
    display.UpdateChar(5, 5, 'A');
    display.UpdateChar(6, 5, 'B');
    display.UpdateChar(6, 6, 'C');
    display.Flush();
    SUCCEED("Buffer flushed");
}