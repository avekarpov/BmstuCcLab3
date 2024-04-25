#pragma once

#include "list_assignment.hpp"

class NBlockV1
{
private:
    class Chunk
    {
    public:
        friend class NBlockV1;

        NListAssignmentPtr listAssignment() const
        {
            return _list_assignment_ptr;
        }

    private:
        NListAssignmentPtr _list_assignment_ptr;
    };

public:
    NBlockV1(NListAssignmentPtr list_assignment_ptr)
    : _list_assignment_ptr { validate(list_assignment_ptr) }
    {}

    void addChunk(NListAssignmentPtr list_assignment_ptr)
    {
        _tail.emplace_back();
        _tail.back()._list_assignment_ptr = validate(list_assignment_ptr);
    }

    NListAssignmentPtr listAssignment() const
    {
        return _list_assignment_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }

private:
    static NListAssignmentPtr validate(NListAssignmentPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null list assignment node" };
        }
        
        return ptr;
    }

private:
    NListAssignmentPtr _list_assignment_ptr;
    std::vector<Chunk> _tail;
};

struct NBlock : std::variant<NBlockV1>
{
    using std::variant<NBlockV1>::variant;
};
using NBlockPtr = std::shared_ptr<NBlock>;

inline NBlockPtr parseBlock(Lexer lexer)
{
    NBlockV1 node { parseListAssignment(lexer) };
    while (not lexer.isEnd())
    {
        node.addChunk(parseListAssignment(lexer));
    }
    return std::make_shared<NBlock>(std::move(node));
}