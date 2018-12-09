#include <iostream>

#include "./Peer.h"
#include "RakNet/Kbhit.h"

using namespace std;
using namespace RakNet;

void Loop(Peer &peer)
{
	while (true)
	{
		peer.Step();

		if (peer.connected)
		{
			char buffer[1024];
			cin.getline(buffer, sizeof(buffer));
			peer.SendText(buffer);
			continue;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		cerr << "usage: " << argv[0] << " remote_url remote_port local_port" << endl;
		return -1;
	}

	const char *remoteUrl = argv[1];
	int remotePort = atoi(argv[2]);
	int localPort = atoi(argv[3]);

	Peer peer(localPort);
	cout << "Listening on " << localPort << endl;

	if (!peer.Start())
	{
		cerr << "Failed to start peer" << endl;
		return -1;
	}

	peer.Connect(remoteUrl, remotePort);

	Loop(peer);
}
