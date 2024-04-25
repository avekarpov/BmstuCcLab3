#include <catch2/catch_all.hpp>

#include <pan_operation.hpp>

TEST_CASE("Pan operation: parse")
{
    SECTION("power")
    {
        const std::string string = "** $";

        Lexer lexer { string };

        const auto node_ptr = parsePanOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NPowerPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("abs")
    {
        const std::string string = "abs $";

        Lexer lexer { string };

        const auto node_ptr = parsePanOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NAbsPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("not")
    {
        const std::string string = "not $";

        Lexer lexer { string };

        const auto node_ptr = parsePanOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NNotPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}