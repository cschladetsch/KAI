#pragma once

#include <KAI/Language/Tau/Generate/GenerateProcess.h>

TAU_BEGIN

namespace Generate
{
    struct GenerateAgent : GenerateProcess
    {
        using GenerateProcess::Node;

        GenerateAgent(const char *in, const char *out);

    protected:
        bool Class(Node const &cl) override;
        bool Property(Node const &prop) override;
        bool Method(Node const &method) override;
        string Prepend() const override;

        string ArgType(string const &text) const override;
        string ReturnType(string const &text) const override;

    private:
        struct Decl;
        void AddAgentBoilerplate(Decl const &agent);
    };
}

TAU_END

