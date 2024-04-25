#include <catch2/catch_all.hpp>

#include <binary_operation.hpp>

TEST_CASE("Bi operation: parse")
{
    SECTION("plus")
    {
        const std::string string = "+ $";

        Lexer lexer { string };

        const auto node_ptr = parseBiOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NBiPlusPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("minus")
    {
        const std::string string = "- $";

        Lexer lexer { string };

        const auto node_ptr = parseBiOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NBiMinusPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("and")
    {
        const std::string string = "& $";

        Lexer lexer { string };

        const auto node_ptr = parseBiOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NBiAndPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}