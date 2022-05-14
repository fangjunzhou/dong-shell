#include <catch2/catch_test_macros.hpp>
#include <unistd.h>
#include "VerticalDisplay.hpp"

static bool redirectResult = false;

int stdoutFd = -1;
FILE *openedFile = NULL;

void RedirectTestOutput(std::string filePath)
{
    stdoutFd = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    openedFile = fopen(filePath.c_str(), "w");
}

void ReopenStdout()
{
    if (stdoutFd == -1)
    {
        throw std::invalid_argument("No file opened.");
    }
    fclose(openedFile);
    dup2(stdoutFd, STDOUT_FILENO);
}

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

    if (!redirectResult)
    {
        std::cin.get();
    }
}

TEST_CASE("VerticalDisplay Flush Buffer", "[VerticalDisplay]")
{
    if (redirectResult)
        RedirectTestOutput("../tests_output/VerticalDisplay_Flush_Buffer.txt");

    std::cout << "\033[2J";

    VerticalDisplay display = VerticalDisplay(NULL, {10, 10});

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "UpdateChar()");
    display.UpdateChar(5, 5, 'A');
    display.UpdateChar(6, 5, 'B');
    display.UpdateChar(6, 6, 'C');

    DisplayDebugInfo(11, 0, "Flush()");
    display.Flush();

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    if (redirectResult)
        ReopenStdout();
}

TEST_CASE("VerticalDisplay Push String", "[VerticalDisplay]")
{
    if (redirectResult)
        RedirectTestOutput("../tests_output/VerticalDisplay_Push_String.txt");

    std::cout << "\033[2J";
    VerticalDisplay display = VerticalDisplay(NULL, {10, 10});

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "PushString()");
    display.PushString("Hello World!\n");
    display.PushString("Hello World2!\n");
    display.PushString("Hello World3!\n");

    DisplayDebugInfo(11, 0, "Flush()");
    display.Flush();

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    if (redirectResult)
        ReopenStdout();
}

TEST_CASE("VerticalDisplay Push String Space", "[VerticalDisplay]")
{
    if (redirectResult)
        RedirectTestOutput("../tests_output/VerticalDisplay_Push_String_Space.txt");

    std::cout << "\033[2J";
    VerticalDisplay display = VerticalDisplay(NULL, {10, 10}, 2);

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "PushString()");
    display.PushString("Hello World!\n");
    display.PushString("Hello World2!\n");
    display.PushString("Hello World3!\n");

    DisplayDebugInfo(11, 0, "Flush()");
    display.Flush();

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    if (redirectResult)
        ReopenStdout();
}

TEST_CASE("VerticalDisplay Resize Console", "[VerticalDisplay]")
{
    if (redirectResult)
        RedirectTestOutput("../tests_output/VerticalDisplay_Resize_Console.txt");

    // Console resize callback.
    eventpp::CallbackList<void(int width, int height)> consoleResizeCallback;

    std::cout << "\033[2J";
    VerticalDisplay display = VerticalDisplay(&consoleResizeCallback, {10, 10}, 2);

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "PushString()");
    display.PushString("Hello World!\n");
    display.PushString("Hello World2!\n");
    display.PushString("Hello World3!\n");
    display.PushString("Hello World4!\n");
    display.PushString("Hello World5!\n");

    DisplayDebugInfo(11, 0, "Flush()");
    display.Flush();

    DisplayDebugInfo(21, 0, "Resize Console");
    consoleResizeCallback(20, 20);

    DisplayDebugInfo(21, 0, "ClearDisplay()");
    display.ClearDisplay();

    if (redirectResult)
        ReopenStdout();
}

TEST_CASE("VerticalDisplay Long Line", "[VerticalDisplay]")
{
    if (redirectResult)
        RedirectTestOutput("../tests_output/VerticalDisplay_Long_Line.txt");

    // Console resize callback.
    eventpp::CallbackList<void(int width, int height)> consoleResizeCallback;

    std::cout << "\033[2J";
    VerticalDisplay display = VerticalDisplay(&consoleResizeCallback, {10, 10}, 2);

    DisplayDebugInfo(11, 0, "ClearDisplay()");
    display.ClearDisplay();

    DisplayDebugInfo(11, 0, "PushString()");
    display.PushString("Hello World!");
    display.PushString("Hello World2!");
    display.PushString("Hello World3!");
    display.PushString("Hello World4!");
    display.PushString("Hello World5!");

    DisplayDebugInfo(11, 0, "Flush()");
    display.Flush();

    DisplayDebugInfo(21, 0, "Resize Console");
    consoleResizeCallback(20, 20);

    DisplayDebugInfo(21, 0, "ClearDisplay()");
    display.ClearDisplay();

    if (redirectResult)
        ReopenStdout();
}