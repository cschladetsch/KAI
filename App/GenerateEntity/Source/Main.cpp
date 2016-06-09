#include <KAI/Language/Tau/Generate.h>
#include <KAI/Language/Tau/Generate/Proxy.h>

using namespace std;
using namespace kai;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cerr << "usage: interface.tau output.h" << endl;
		return 1;
	}

	tau::Generate::Proxy p(argv[1], argv[2]);
	if (p.Failed)
	{
		cerr << "Error: " << p.Error << endl;
		return 1;
	}

	return 0;
}

