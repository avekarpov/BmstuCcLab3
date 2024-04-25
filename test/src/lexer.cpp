#include <catch2/catch_all.hpp>

#include <lexer.hpp>

TEST_CASE("Lexer: iteration")
{
    /*
    const std::string string { "1 2 3" };

    Lexer lexer { string };

    SECTION("form start")
    {
        for (size_t i = 0; i < string.size(); ++i)
        {
            CHECK(i == lexer.pos());
            CHECK(string[i] == lexer.getNext());
        }

        CHECK(lexer.isEnd());
    }

    SECTION("from other lexer")
    {
        lexer.getNext();

        auto additional_lexer = lexer;
        REQUIRE(lexer.pos() == additional_lexer.wasAt());
        for (size_t i = 1; i < string.size(); ++i)
        {
            CHECK(i == lexer.pos());
            CHECK(string[i] == additional_lexer.getNext());
        }

        CHECK(lexer.isEnd());      
    }

    SECTION("reset")
    {
        lexer.getNext();

        auto additional_lexer = lexer;
        REQUIRE(lexer.pos() == additional_lexer.wasAt());
        while (not additional_lexer.isEnd()) { lexer.getNext(); }

        REQUIRE(lexer.isEnd());

        additional_lexer.reset();

        CHECK(not lexer.isEnd());
        CHECK(lexer.pos() == 1);
    }

    SECTION("rollback")
    {
        lexer.getNext();
        lexer.rollback(1);
    
        CHECK(lexer.pos() == 0);
    }
}

TEST_CASE("Lexer: getNextToken")
{
    const std::string string { "one two three" };

    Lexer lexer { string };

    SECTION("don't reset")
    {
        CHECK("one" == getNextToken(lexer));
        CHECK("two" == getNextToken(lexer));
        CHECK("three" == getNextToken(lexer));
    }
    */
}
