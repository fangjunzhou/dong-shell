#include <catch2/catch_test_macros.hpp>
#include "CommandHandler.hpp"

TEST_CASE("Split Command", "[SplitCommand]")
{
    CommandHandler handler = CommandHandler(0, 0, NULL);

    SECTION("ls -la")
    {
        std::vector<std::string> &res = handler.SplitCommand("ls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls\t-la")
    {
        std::vector<std::string> &res = handler.SplitCommand("ls\t-la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls \t-la")
    {
        std::vector<std::string> &res = handler.SplitCommand("ls \t-la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls -la ")
    {
        std::vector<std::string> &res = handler.SplitCommand("ls -la ");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION(" ls -la")
    {
        std::vector<std::string> &res = handler.SplitCommand(" ls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("\tls -la")
    {
        std::vector<std::string> &res = handler.SplitCommand("\tls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }
}