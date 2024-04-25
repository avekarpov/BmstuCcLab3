#include <catch2/catch_all.hpp>

#include <factor.hpp>

TEST_CASE("Factor: parse")
{
    SECTION("V1")
    {
        SECTION("only primary")
        {
            const std::string string = "#42 $";

            Lexer lexer { string };

            const auto node_ptr = parseFactor(lexer);
            REQUIRE(node_ptr != nullptr);
            std::visit(
                [](auto node)
                {
                    CHECK(std::is_same_v<decltype(node), NFactorV1>);
                    if constexpr (std::is_same_v<decltype(node), NFactorV1>)
                    {
                        CHECK(node.primary() != nullptr);
                        CHECK(node.tail().empty());
                    }
                },
                *node_ptr
            );
            CHECK(lexer.peekNext() == '$');
        }

        SECTION("with tail")
        {
            const std::string string = "#42 ** @alfred $";

            Lexer lexer { string };

            const auto node_ptr = parseFactor(lexer);
            REQUIRE(node_ptr != nullptr);
            std::visit(
                [](auto node)
                {
                    CHECK(std::is_same_v<decltype(node), NFactorV1>);
                    if constexpr (std::is_same_v<decltype(node), NFactorV1>)
                    {
                        CHECK(node.primary() != nullptr);
                        REQUIRE(node.tail().size() == 1);
                        CHECK(node.tail().front().power() != nullptr);
                        CHECK(node.tail().front().primary() != nullptr);
                    }
                },
                *node_ptr
            );
            CHECK(lexer.peekNext() == '$');
        }
    }

    // TODO: V2, V3
}