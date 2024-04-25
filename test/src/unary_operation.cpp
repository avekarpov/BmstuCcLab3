#include <catch2/catch_all.hpp>

#include <unary_operation.hpp>

TEST_CASE("Un operation: parse")
{
    SECTION("plus")
    {
        const std::string string = "+ $";

        Lexer lexer { string };

        const auto node_ptr = parseUnOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NUnPlusPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("minus")
    {
        const std::string string = "- $";

        Lexer lexer { string };

        const auto node_ptr = parseUnOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NUnMinusPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}