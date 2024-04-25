#pragma once

#include "primary.hpp"
#include "pan_operation.hpp"

class NPrimary;
using NPrimaryPtr = std::shared_ptr<NPrimary>;
NPrimaryPtr parsePrimary(Lexer lexer);

class NFactorV1
{
private:
    class Chunk
    {
    public:
        friend class NFactorV1;

        NPowerPtr power() const
        {
            return _power_ptr;
        }

        NPrimaryPtr primary() const
        {
            return _primary_ptr;
        }

    private:
        NPowerPtr _power_ptr;
        NPrimaryPtr _primary_ptr;
    };

public:
    NFactorV1(NPrimaryPtr primary_ptr)
    : _primary_ptr { validate(primary_ptr) }
    {}

    void addChunk(NPowerPtr power_ptr, NPrimaryPtr primary_ptr)
    {
        _tail.emplace_back();
        _tail.back()._power_ptr = validate(power_ptr);
        _tail.back()._primary_ptr = validate(primary_ptr);
    }

    NPrimaryPtr primary() const
    {
        return _primary_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }

private:
    static NPrimaryPtr validate(NPrimaryPtr primary_ptr)
    {
        if (primary_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null primary node" };
        }

        return primary_ptr;
    }

    static NPowerPtr validate(NPowerPtr power_ptr)
    {
        if (power_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null power node" };
        }

        return power_ptr;
    }

public:
    NPrimaryPtr _primary_ptr;
    std::vector<Chunk> _tail;
};

class NFactorV2
{
public:
    NFactorV2(NAbsPtr abs_ptr, NPrimaryPtr primary_ptr)
    : _abs_ptr { validate(abs_ptr) }
    , _primary_ptr { validate(primary_ptr) }
    {}

    NAbsPtr abs() const
    {
        return _abs_ptr;
    }

    NPrimaryPtr primary() const
    {
        return _primary_ptr;
    }

private:
    static NAbsPtr validate(NAbsPtr abs_ptr)
    {
        if (abs_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null abs node" };
        }

        return abs_ptr;
    }

    static NPrimaryPtr validate(NPrimaryPtr primary_ptr)
    {
        if (primary_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null primary node" };
        }

        return primary_ptr;
    }

private:
    NAbsPtr _abs_ptr;
    NPrimaryPtr _primary_ptr;
};

class NFactorV3
{
public:
    NFactorV3(NNotPtr not_ptr, NPrimaryPtr primary_ptr)
    : _not_ptr { validate(not_ptr) }
    , _primary_ptr { validate(primary_ptr) }
    {}

    NNotPtr nix() const
    {
        return _not_ptr;
    }

    NPrimaryPtr primary() const
    {
        return _primary_ptr;
    }

private:
    static NNotPtr validate(NNotPtr not_ptr)
    {
        if (not_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null not node" };
        }

        return not_ptr;
    }

    static NPrimaryPtr validate(NPrimaryPtr primary_ptr)
    {
        if (primary_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null primary node" };
        }

        return primary_ptr;
    }

private:
    NNotPtr _not_ptr;
    NPrimaryPtr _primary_ptr;
};

struct NFactor : std::variant<NFactorV1, NFactorV2, NFactorV3>
{
    using std::variant<NFactorV1, NFactorV2, NFactorV3>::variant;
};
using NFactorPtr = std::shared_ptr<NFactor>;

inline NFactorPtr parseFactor(Lexer lexer)
{
    if (const auto primary_ptr = parsePrimary(lexer); primary_ptr != nullptr)
    {
        NFactorV1 node { primary_ptr };
        for (auto power_ptr = parsePower(lexer); power_ptr != nullptr; power_ptr = parsePower(lexer))
        {
            node.addChunk(power_ptr, parsePrimary(lexer));
        }
        return std::make_shared<NFactor>(std::move(node));
    }
    if (const auto abs_ptr = parseAbs(lexer); abs_ptr != nullptr)
    {
        NFactorV2 node { abs_ptr, parsePrimary(lexer) };
        return std::make_shared<NFactor>(std::move(node));
    }
    if (const auto not_ptr = parseNot(lexer); not_ptr != nullptr)
    {
        NFactorV3 node { not_ptr, parsePrimary(lexer) };
        return std::make_shared<NFactor>(std::move(node));
    }
    
    return nullptr;
}