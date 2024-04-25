#include <catch2/catch_all.hpp>

#include <simple_statement.hpp>

TEST_CASE("Simple statement: parse")
{
    SECTION("without un operation and tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom $";

        Lexer lexer { string };

        const auto node_ptr = parseSimpleStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.unOperation() == nullptr);
                CHECK(node.term() != nullptr);
                CHECK(node.tail().empty());
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("without tail")
    {
        const std::string string = "- #42 ** @alfred * abs @tom $";

        Lexer lexer { string };

        const auto node_ptr = parseSimpleStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.unOperation() != nullptr);
                CHECK(node.term() != nullptr);
                CHECK(node.tail().empty());
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("with tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom & @mem mod #7 $";

        Lexer lexer { string };

        const auto node_ptr = parseSimpleStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.unOperation() == nullptr);
                CHECK(node.term() != nullptr);
                REQUIRE(node.tail().size() == 1);
                CHECK(node.tail().front().biOperation() != nullptr);
                CHECK(node.tail().front().term() != nullptr);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("with un operation and tail")
    {
        const std::string string = "- #42 ** @alfred * abs @tom & @mem mod #7 $";

        Lexer lexer { string };

        const auto node_ptr = parseSimpleStatement(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.unOperation() != nullptr);
                CHECK(node.term() != nullptr);
                REQUIRE(node.tail().size() == 1);
                CHECK(node.tail().front().biOperation() != nullptr);
                CHECK(node.tail().front().term() != nullptr);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}