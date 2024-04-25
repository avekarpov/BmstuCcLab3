#pragma once

#include <string>
#include <memory>
#include <format>
#include <cassert>

class Lexer
{
public:
    Lexer (const Lexer &other)
    : _string_ptr { other._string_ptr }
    , _pos_ptr { other._pos_ptr }
    , _was_at { *_pos_ptr }
    {}

    Lexer (std::string string)
    : _string_ptr { std::make_shared<const std::string>(std::move(string)) }
    , _pos_ptr { std::make_shared<size_t>(0) }
    , _was_at { 0 }
    {}

    bool isEnd() const
    {
        return *_pos_ptr == _string_ptr->size();
    }

    char getNext()
    {
        assert(not isEnd());

        ++(*_pos_ptr);
        
        return _string_ptr->operator[]((*_pos_ptr) - 1);        
    }

    void next()
    {
        assert(not isEnd());

        ++(*_pos_ptr);
    }

    char peekNext()
    {
        assert(not isEnd());

        return _string_ptr->operator[](*_pos_ptr);
    }

    void rollback(const size_t diff = 1)
    {
        assert(not isEnd());

        *_pos_ptr -= diff;
    }

    size_t pos() const
    {
        return *_pos_ptr;
    }

    size_t wasAt() const
    {
        return _was_at;
    }

    void reset()
    {
        *_pos_ptr = _was_at;
    }

    friend std::string_view getNextToken(Lexer lexer, const std::string_view until_one_of);
    friend std::string_view peekNextToken(Lexer lexer, const std::string_view until_one_of);
    friend void next(Lexer lexer, const size_t size);
    friend void skip(Lexer lexer, const std::string_view characters);

private:
    const std::shared_ptr<const std::string> _string_ptr;
    const std::shared_ptr<size_t> _pos_ptr;
    const size_t _was_at;
};

inline std::string_view getNextToken(Lexer lexer, const std::string_view until_one_of = " \n")
{
    const auto token = peekNextToken(lexer, until_one_of);

    *lexer._pos_ptr += token.size();

    return token;
}

inline std::string_view peekNextToken(Lexer lexer, const std::string_view until_one_of = " \n")
{
    const auto next_space = lexer._string_ptr->find_first_of(until_one_of, *lexer._pos_ptr);

    std::string_view next_token
    {
        lexer._string_ptr->begin() + *lexer._pos_ptr,
        lexer._string_ptr->begin() +
        (
            next_space != std::string::npos
            ? next_space
            : lexer._string_ptr->size()
        )
    };

    return next_token;
}

inline void next(Lexer lexer, const size_t size)
{
    assert(*lexer._pos_ptr + size < lexer._string_ptr->size());

    *lexer._pos_ptr += size;
}

inline void skip(Lexer lexer, const std::string_view characters)
{
    while (not lexer.isEnd() && characters.find(lexer.peekNext()) != std::string::npos)
    {
        lexer.next();
    }
}
