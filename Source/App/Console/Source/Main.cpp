#include <KAI/Console/Console.h>
#include <iostream>

using namespace std;

USING_NAMESPACE_KAI

int main(int argc, char **argv)
{
    KAI_UNUSED_2(argc, argv);

    cout << "KAI v0.1" << endl;

    Console console;
    console.SetLanguage(Language::Pi);

    // the higher the number, the greater the verbosity of debug output
    Process::trace = 0;
    console.GetExecutor()->SetTraceLevel(0);

    // start the REPL
    return console.Run();
}
