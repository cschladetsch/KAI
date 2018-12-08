#pragma once

#include <KAI/Language/Tau/Generate/GenerateProcess.h>

TAU_BEGIN

namespace Generate
{
    struct Proxy : GenerateProcess
    {
        using GenerateProcess::Node;

         Proxy(const char *inputFile, const char *outputFile);
        // Proxy();
        // bool Generate(const char *inputFile, const char *outputFile);

    protected:
        virtual string Prepend() const override;
        virtual bool Class(Node const &cl) override;
        virtual bool Property(Node const &prop) override;
        virtual bool Method(Node const &method) override;
        virtual string ArgType(string const &text) const override;
        virtual string ReturnType(string const &text) const override;

    private:
        struct ProxyDecl;
        void AddProxyBoilerplate(ProxyDecl const &);
        void MethodBody(const string &returnType, const Node::ChildrenType &args, const string &name);
        void MethodDecl(const string &returnType, const Node::ChildrenType &args, const string &name);
    };
}

TAU_END

