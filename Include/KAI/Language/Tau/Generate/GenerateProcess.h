#pragma once

#include <KAI/Language/Tau/TauParser.h>

#include <strstream>
#include <string>

TAU_BEGIN

namespace Generate
{
    using std::string;
    using std::stringstream;
    using std::shared_ptr;

    // common base for proxy and agent generation
    struct GenerateProcess : Process
    {
        typedef TauParser::AstNode Node;

    public:
        virtual ~GenerateProcess()  { }

        shared_ptr<TauParser> Parse(const char *fileName) const;
        bool Generate(const char *inputFile, const char *outputFile);
        bool Generate(TauParser const &p, const char *fileName);

    protected:
        bool Module(TauParser const &);
        virtual bool Namespace(Node const &ns);

        virtual bool Class(Node const &cl) = 0;
        virtual bool Property(Node const &prop) = 0;
        virtual bool Method(Node const &method) = 0;
        virtual string Prepend() const = 0;

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

