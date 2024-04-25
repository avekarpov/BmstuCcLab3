#include <catch2/catch_all.hpp>

#include <term.hpp>

TEST_CASE("Term: parse")
{
    SECTION("only factor")
    {
        const std::string string = "#42 ** @alfred $";

        Lexer lexer { string };

        const auto node_ptr = parseTerm(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.factor() != nullptr);
                CHECK(node.tail().empty());
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("with tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom $";

        Lexer lexer { string };

        const auto node_ptr = parseTerm(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.factor() != nullptr);
                REQUIRE(node.tail().size() == 1);
                CHECK(node.tail().front().mdmrOperation() != nullptr);
                CHECK(node.tail().front().factor() != nullptr);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}