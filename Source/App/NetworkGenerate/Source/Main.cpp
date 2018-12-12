#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "KAI/Language/Tau/Generate/Proxy.h"
#include "KAI/Language/Tau/Generate/Agent.h"

using namespace std;
using namespace kai;
using namespace boost::filesystem;
using namespace boost::program_options;

enum Generate
{
    None = 0,
    Agent = 1,
    Proxy = 2,
    Both = Agent | Proxy,
};

int main(int argc,const char* const argv[])
{
    options_description desc("Options");

    path out_dir = ".";
    path agent_out_dir = ".";
    path proxy_out_dir = ".";
    string agentOutputDir = ".";
    string proxyOutputDir = ".";

    string proxyName = "%s.proxy.h";
    string agentName = "%s.agent.h";

    desc.add_options()
        ("help", "Talk to Christian")
        ("input", value<path>(), "Input TAU file")
        ("proxy_dir", value<path>(&proxy_out_dir)->default_value(proxyOutputDir), "Set output dir for proxy")
        ("agent_dir", value<path>(&agent_out_dir)->default_value(agentOutputDir), "Set output dir for agent")
        ("proxy_name", value<path>(&proxy_out_dir)->default_value(proxyName), "Set output name for proxy")
        ("agent_name", value<path>(&agent_out_dir)->default_value(agentName), "Set output name for agent")
        ("out", value<path>(&out_dir)->default_value("."), "Set output dir for both agent and proxy")
        ("start", value<path>(), "File included before anything else")
        ("pre", value<path>(), "File included after start and before definition")
        ("post", value<path>(), "File included after definition")
        ;

    positional_options_description p;
    p.add("input", -1);

    variables_map vm;
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);

    if (vm.count("input") != 1)
    {
        cerr << desc;
        return 1;
    }

    auto input = vm["input"].as<path>();
    string outputProxy = "out.proxy.h";//(path(proxyOutputDir).append(vm["proxy_dir"]).append(vm["proxy_name"]));
    string outputAgent = "out.agent.h";//(path(proxyOutputDir).append(vm["proxy_dir"]).append(vm["proxy_name"]));

    // Error because boost::path.c_str() return wchar_t *
#if 0
    if (!outputProxy.empty())
    {
        tau::Generate::Proxy proxy(input.c_str(), outputProxy.c_str());
        if (proxy.Failed)
        {
            cerr << "ProxyGenError: " << proxy.Error << endl;
            return 1;
        }
        cout << "Wrote proxy code to " << outputProxy << endl;
    }

    if (!outputAgent.empty())
    {
        tau::Generate::Agent agent(input.c_str(), outputAgent.c_str());
        if (agent.Failed)
        {
            cerr << "AgentGenError: " << agent.Error << endl;
            return 1;
        }
        cout << "Wrote agent code to " << outputAgent << endl;
    }
#endif

    return 0;
}

