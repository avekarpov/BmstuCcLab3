#include <catch2/catch_all.hpp>

#include <number.hpp>

TEST_CASE("Number: parse")
{
    const unsigned long number = 42;
    const std::string string = std::format("#{} $", number);

    Lexer lexer { string };

    const auto node_ptr = parseNumber(lexer);
    REQUIRE(node_ptr != nullptr);
    std::visit(
        [&](auto node)
        {
            CHECK(node.value() == number);
        },
        *node_ptr
    );
    CHECK(lexer.peekNext() == '$');
}