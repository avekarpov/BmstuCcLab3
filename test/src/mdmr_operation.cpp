#include <catch2/catch_all.hpp>

#include <mdmr_operation.hpp>

TEST_CASE("Mdmr operation: parse")
{
    SECTION("mul")
    {
        const std::string string = "* $";

        Lexer lexer { string };

        const auto node_ptr = parseMdmrOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NMulPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("div")
    {
        const std::string string = "/ $";

        Lexer lexer { string };

        const auto node_ptr = parseMdmrOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NDivPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("mod")
    {
        const std::string string = "mod $";

        Lexer lexer { string };

        const auto node_ptr = parseMdmrOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NModPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }

    SECTION("rem")
    {
        const std::string string = "rem $";

        Lexer lexer { string };

        const auto node_ptr = parseMdmrOperation(lexer);
        REQUIRE(node_ptr != nullptr);
        std::visit(
            [](auto node)
            {
                CHECK(std::is_same_v<decltype(node), NRemPtr>);
            },
            *node_ptr
        );
        CHECK(lexer.peekNext() == '$');
    }
}