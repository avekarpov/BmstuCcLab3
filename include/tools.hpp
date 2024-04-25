#pragma once

#define OPERATION(__NAME__) \
struct N##__NAME__##V1 {}; \
using N##__NAME__ = std::variant<N##__NAME__##V1>; \
using N##__NAME__##Ptr = std::shared_ptr<N##__NAME__>

#define GROUP_OPERATIONS(__NAME__, ...) \
using N##__NAME__ = std::variant<__VA_ARGS__>; \
using N##__NAME__##Ptr = std::shared_ptr<N##__NAME__>

#define PARSE_OPERATION(__NAME__, __TOKEN__, __FOLOW_NEXT__) \
inline N##__NAME__##Ptr parse##__NAME__(Lexer lexer) \
{ \
    static constexpr std::string_view token = (__TOKEN__); \
\
    if (peekNextToken(lexer, (__FOLOW_NEXT__)) != token) \
    { \
        return nullptr; \
    } \
    next(lexer, token.size()); \
\
    skip(lexer, " \n"); \
\
    return std::make_shared<N##__NAME__>(); \
} \
struct __parse##__NAME__##__ {}

#define PARSE_OPERTATION_VARIANT(__NAME__, __GROUP_NAME__) \
if (const auto ptr = parse##__NAME__(lexer); ptr != nullptr) \
{ \
    N##__GROUP_NAME__ node { ptr }; \
    return std::make_shared<N##__GROUP_NAME__>(std::move(node)); \
}

#define SIMPLE_VISIT(__NAME__, __LABEL_VALUE__) \
std::string visit(const N##__NAME__##Ptr ptr, const size_t id) \
{ \
    assert(ptr != nullptr); \
\
    return std::visit( \
        [&id](const N##__NAME__##V1 node) -> std::string \
        { \
            return std::format("{} [label = \"{}\"]", id, (__LABEL_VALUE__)); \
        }, \
        *ptr \
    ); \
} \
struct __visit##__NAME__##__ {}
