#include <iostream>

#include "KAI/Console/Console.h"
#include "KAI/Network/Peer.h"

using namespace std;
using namespace kai;
using namespace Bin;

struct Peer : kai::Peer
{
    bool Start()
    {
        //return _peer.Start(ReceivePacket);
		return false;
    }

    bool Connect(String host, int port)
    {
        return _peer.Connect(host.c_str(), port);
    }

    void Send(String text)
    {
        _peer.SendText(text.c_str());
    }

    //void ReceivePacket(RakNet::Pac)
    //{
    //}

    kai::Peer _peer;
};

Console console;

string Kai_VersionString()
{
    stringstream str;
    str << KAI_VERSION_MAJOR << '.' << KAI_VERSION_MINOR << '.' << KAI_VERSION_PATCH;
    return str.str();
}

int main(int argc, char **argv)
{
    KAI_UNUSED_2(argc, argv);

    cout << "KAI v" << Kai_VersionString() << endl << endl;

    console.SetLanguage(Language::Pi);

    // the higher the number, the greater the verbosity of debug output for language systems
    Process::trace = 0;

    // the higher the number, the greater the verbosity of debug output for the Executor
    console.GetExecutor()->SetTraceLevel(0);

    // start the REPL
    return console.Run();
}
