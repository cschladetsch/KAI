#pragma once

#include <KAI/Language/Tau/Generate/GenerateProcess.h>

TAU_BEGIN

namespace Generate
{
    struct GenerateProxy : GenerateProcess
    {
        using GenerateProcess::Node;

        GenerateProxy(const char *inputFile, const char *outputFile);

    protected:
        string Prepend() const override;
        bool Namespace(Node const &cl) override;
        bool Class(Node const &cl) override;
        bool Property(Node const &prop) override;
        bool Method(Node const &method) override;
        string ArgType(string const &text) const override;
        string ReturnType(string const &text) const override;

        bool Generate(TauParser const &p, const char *fileName) override;

    private:
        struct ProxyDecl;
        void AddProxyBoilerplate(ProxyDecl const &);
        void MethodBody(const string &returnType, const Node::ChildrenType &args, const string &name);
        void MethodDecl(const string &returnType, const Node::ChildrenType &args, const string &name);
    };
}

TAU_END
