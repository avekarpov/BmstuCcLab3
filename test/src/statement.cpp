#include <catch2/catch_all.hpp>

#include <statement.hpp>

TEST_CASE("Statement: parse")
{
    SECTION("without tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom $";

        Lexer lexer { string };

        const auto node_ptr = parseStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.relation() != nullptr);
                CHECK(node.tail().empty());
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("with tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom xor @mem mod #7 >= #10 $";

        Lexer lexer { string };

        const auto node_ptr = parseStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.relation() != nullptr);
                REQUIRE(node.tail().size() == 1);
                CHECK(node.tail().front().logicalOperation() != nullptr);
                CHECK(node.tail().front().relation() != nullptr);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}
