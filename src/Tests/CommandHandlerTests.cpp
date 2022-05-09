#include <catch2/catch_test_macros.hpp>
#include "CommandHandler.hpp"

TEST_CASE("Split Command", "[SplitCommand]")
{
    SECTION("ls -la")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand("ls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls\t-la")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand("ls\t-la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls \t-la")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand("ls \t-la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("ls -la ")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand("ls -la ");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION(" ls -la")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand(" ls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }

    SECTION("\tls -la")
    {
        std::vector<std::string> &res = CommandHandler::SplitCommand("\tls -la");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "ls");
        REQUIRE(res[1] == "-la");
    }
}