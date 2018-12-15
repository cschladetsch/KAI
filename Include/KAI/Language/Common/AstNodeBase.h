#pragma once

#include <memory>
#include <vector>
#include <string>
#include <sstream>

// #include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object.h>
#include <KAI/Language/Common/AstNodeBase.h>

KAI_BEGIN

// A common AST Node, given the Tokens to use, and the
// enumeration type Enum that indicates what type of AST it is
template <class EToken, class AstEnumType>
class AstNodeBase
{
public:
    typedef EToken Token;
    typedef AstNodeBase<Token, AstEnumType> AstNode;
    typedef std::shared_ptr<AstNode> AstNodePtr;
    typedef typename AstEnumType::Enum Enum;
    typedef AstNodeBase<Token, AstEnumType> Self;
    typedef std::shared_ptr<Self> Child;
    typedef std::vector<Child> ChildrenType;

    AstNodeBase() : _astType((Enum)0) { }
    AstNodeBase(Enum e) : _astType(e) { }
    AstNodeBase(Enum e, Token t) : _astType(e), _token(t) { }
    AstNodeBase(Token const &t) : _astType(AstEnumType::TokenType), _token(t) { }

    const Child &GetChild(size_t n) const { return GetChildren()[n]; }
    const ChildrenType &GetChildren() const { return _children; }
    Enum GetType() const { return _astType; }

    const Token &GetToken() const { return _token; }
    std::string GetTokenText() const { return std::move(_token.Text()); }

    std::string ToString() const
    {
        std::stringstream out;
        out << AstEnumType::ToString(_astType) << ": " << _token.ToString() << std::ends;
        //out << _token.ToString() << std::ends;
        return out.str();
    }

    std::string Text() const
    {
        return std::move(_token.Text());
    }

    void Add(AstNodePtr node)
    {
        _children.push_back(node);
    }

    void Add(Enum type, Object content)
    {
        _children.push_back(std::make_shared<Self>(type, content));
    }

    void Add(Token const &tok)
    {
        Add(std::make_shared<Self>(tok));
    }

    friend std::ostream &operator<<(std::ostream &out, Self const &node)
    {
        return out << node.ToString();
    }

    size_t NumChildren() const
    {
        return _children.size();
    }

protected:
    Enum _astType;
    Token _token;
    ChildrenType _children;

private:
    void PrintTree(std::ostream &out, int level, Self const &self)
    {
        out << ToString() << std::endl;
        std::string indent(4, ' ');
        for (const auto &ch : GetChildren())
        {
            out << indent << ch << std::endl;
            PrintTree(out, level + 1, ch);
        }
    }
};

KAI_END

