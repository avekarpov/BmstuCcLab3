#include <catch2/catch_all.hpp>

#include <primary.hpp>

TEST_CASE("Primary: parse")
{
    SECTION("number")
    {
        const std::string string = "#42 $";;

        Lexer lexer { string };

        const auto node_ptr = parsePrimary(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [&](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NNumberPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("name")
    {
        const std::string string = "@alfred $";

        Lexer lexer { string };

        const auto node_ptr = parsePrimary(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NNamePtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("statement")
    {
        // TODO: implement
    }
}
