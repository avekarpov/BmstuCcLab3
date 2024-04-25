#include <catch2/catch_all.hpp>

#include <name.hpp>

TEST_CASE("Name: parse")
{
    const std::string name { "alfred" };
    const std::string string = std::format("@{} $", name);

    Lexer lexer { string };

    const auto node_ptr = parseName(lexer);
    REQUIRE(node_ptr != nullptr);
    std::visit(
        [&](auto node)
        {
            CHECK(node.value() == name);
        },
        *node_ptr
    );
    CHECK(lexer.peekNext() == '$');
}