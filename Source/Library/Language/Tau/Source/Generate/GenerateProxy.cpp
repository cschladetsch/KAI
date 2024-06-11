#include <KAI/Language/Tau/Generate/GenerateProxy.h>
#include <KAI/Language/Tau/TauParser.h>
#include <KAI/Network/Proxy.h>

using namespace std;

TAU_BEGIN

namespace Generate {
GenerateProxy::GenerateProxy(const char *input, string &output) {
    GenerateProcess::Generate(input, output);
}

bool GenerateProxy::Generate(TauParser const &p, string &output) {
    auto const &root = p.GetRoot();
    if (root->GetType() != TauAstEnumType::Module)
        return Fail("Expected a Module");

    for (const auto &ch : root->GetChildren()) {
        if (ch->GetType() != TauAstEnumType::Namespace)
            return Fail("Namespace expected");

        if (!Namespace(*ch)) return false;
    }

    stringstream str;
    str << Prepend() << "\n" << Output().str() << ends;
    return !Failed;
}

string GenerateProxy::Prepend() const {
    return string("#include <KAI/Network/ProxyDecl.h>\n\n");
}

bool GenerateProxy::Namespace(Node const &ns) {
    StartBlock(string("namespace ") + ns.GetToken().Text());
    for (auto const &ch : ns.GetChildren()) {
        switch (ch->GetType()) {
            case TauAstEnumType::Namespace:
                if (!Namespace(*ch)) return false;

            case TauAstEnumType::Class:
                if (!Class(*ch)) return false;

            default:
                return Fail("[Internal] Unexpected %s in namespace",
                            TauAstEnumType::ToString(ch->GetType()));
        }
    }

    EndBlock();
    return true;
}

bool GenerateProxy::Class(Node const &cl) {
    StartBlock(string("class ") + cl.GetToken().Text());

    for (const auto &member : cl.GetChildren()) {
        switch (member->GetType()) {
            case TauAstEnumType::Class:
                return Class(*member);

            case TauAstEnumType::Property:
                if (!Property(*member)) return false;

            case TauAstEnumType::Method:
                if (!Method(*member)) return false;

            default:
                return Fail("Invalid class member: %s",
                            TauAstEnumType::ToString(member->GetType()));
        }
    }

    EndBlock();
    return true;
}

bool GenerateProxy::Property(Node const &prop) {
    Output() << ReturnType(prop.GetTokenText())
             << prop.GetChild(1)->GetTokenText() << ';' << EndLine();
    return true;
}

bool GenerateProxy::Method(Node const &method) {
    auto const &rt = method.GetChild(0);
    auto const &args = method.GetChild(1);
    //        auto const &konst = method.GetChild(2);

    Output() << ReturnType(rt->GetTokenText()) << " " << method.GetTokenText()
             << "(";
    bool first = true;
    for (auto const &a : args->GetChildren()) {
        if (!first) Output() << ", ";

        auto &ty = a->GetChild(0);
        auto &id = a->GetChild(1);
        Output() << ArgType(ty->GetTokenText()) << " " << id->GetTokenText();

        first = false;
    }

    return (Output() << ");" << EndLine()).good();
}

string GenerateProxy::ReturnType(string const &text) const {
    return string("IFuture<") + text + "> ";
}

string GenerateProxy::ArgType(string const &text) const { return text; }
}  // namespace Generate

TAU_END

// EOF
