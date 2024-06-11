#pragma once

#include <KAI/Language/Tau/TauParser.h>

#include <string>

TAU_BEGIN

namespace Generate {
using std::shared_ptr;
using std::string;
using std::stringstream;

// common base for proxy and agent generation
struct GenerateProcess : Process {
   public:
    typedef TauParser::AstNode Node;

   public:
    virtual ~GenerateProcess() = default;

    bool Generate(const char *input, string &output);

   protected:
    static string CommonPrepend();

    shared_ptr<TauParser> Parse(const char *input) const;

    virtual bool Generate(TauParser const &p, string &output) = 0;

    virtual bool Module(TauParser const &);
    virtual bool Namespace(Node const &ns);
    virtual bool Class(Node const &cl);
    virtual bool Property(Node const &prop);
    virtual bool Method(Node const &method);
    virtual string Prepend() const;

    stringstream &StartBlock(const string &name = "");
    string EndLine() const;
    void EndBlock();

    virtual string ArgType(string const &text) const = 0;
    virtual string ReturnType(string const &text) const = 0;

    stringstream &Output() { return _str; }

   private:
    stringstream _str;
    int _indentation = 0;
};
}  // namespace Generate

TAU_END

// EOF
