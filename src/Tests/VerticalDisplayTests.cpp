#include <catch2/catch_test_macros.hpp>
#include "VerticalDisplay.hpp"

void DisplayDebugInfo(int row, int col, std::string info)
{
    // Save cursor.
    std::cout << "\033[s";
    // Move to target position.
    std::cout << "\033[" << row << ";" << col << "H";
    // Erase line.
    std::cout << "\033[0K";
    std::cout << info;
    // Restore cursor position.
    std::cout << "\033[u";
}

TEST_CASE("VerticalDisplay Flush Buffer", "[VerticalDisplay]")
{
    std::cout << "\033[2J";

    VerticalDisplay display = VerticalDisplay(NULL, {10, 10});

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    std::cin.get();
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "UpdateChar()");
    std::cin.get();
    display.UpdateChar(5, 5, 'A');
    display.UpdateChar(6, 5, 'B');
    display.UpdateChar(6, 6, 'C');

    DisplayDebugInfo(11, 0, "Flush()");
    std::cin.get();
    display.Flush();

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    std::cin.get();
    display.ClearDisplay();

    SUCCEED("Buffer flushed");
}

TEST_CASE("VerticalDisplay Push String", "[VerticalDisplay]")
{
    std::cout << "\033[2J";
    VerticalDisplay display = VerticalDisplay(NULL, {10, 10});

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    std::cin.get();
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "PushString()");
    std::cin.get();
    display.PushString("Hello World!\n");
    display.PushString("你好世界！\n");

    DisplayDebugInfo(11, 0, "Flush()");
    std::cin.get();
    display.Flush();

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    std::cin.get();
    display.ClearDisplay();
}