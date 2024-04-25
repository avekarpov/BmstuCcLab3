#include <catch2/catch_all.hpp>

#include <relation.hpp>

TEST_CASE("Relation: parse")
{
    SECTION("without tail")
    {
        const std::string string = "#42 ** @alfred * abs @tom $";

        Lexer lexer { string };

        const auto node_ptr = parseRelation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.simpleStatement() != nullptr);
                CHECK(node.tail().empty());
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION ("with tail")
    {
        const std::string string = "- #42 ** @alfred * abs @tom & @mem mod #7 >= #10 $";

        Lexer lexer { string };

        const auto node_ptr = parseRelation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(node.simpleStatement() != nullptr);
                REQUIRE(node.tail().size() == 1);
                CHECK(node.tail().front().relationOperation() != nullptr);
                CHECK(node.tail().front().simpleStatement() != nullptr);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}