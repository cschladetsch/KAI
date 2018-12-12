#pragma once

#include <string>
#include <KAI/Language/Tau/TauParser.h>

TAU_BEGIN

namespace Generate
{
    using std::string;
    using std::stringstream;
    using std::shared_ptr;

    // common base for proxy and agent generation
    struct GenerateProcess : Process
    {
    public:
        typedef TauParser::AstNode Node;

    public:
        virtual ~GenerateProcess() = default;

        shared_ptr<TauParser> Parse(const char *fileName) const;
        bool Generate(const char *inputFile, const char *outputFile);
        virtual bool Generate(TauParser const &p, const char *fileName) = 0;

    protected:

        virtual bool Module(TauParser const &);
        virtual bool Namespace(Node const &ns);
        virtual bool Class(Node const &cl);
        virtual bool Property(Node const &prop);
        virtual bool Method(Node const &method);
        virtual string Prepend() const;

    protected:
        string CommonPrepend();
        stringstream &StartBlock(const string &text = "");
        string EndLine() const;
        void EndBlock();

        virtual string ArgType(string const &text) const = 0;
        virtual string ReturnType(string const &text) const = 0;

    protected:
        stringstream _str;
        int _indentation = 0;
    };
}

TAU_END

