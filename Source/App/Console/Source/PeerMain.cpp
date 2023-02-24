#if 0
#include <iostream>

#include "KAI/Network/Peer.h"
#include "RakNet/Kbhit.h"

using namespace std;
using namespace RakNet;

KAI_BEGIN

//void Loop(Peer &peer)
//{
//	while (true)
//	{
//		peer.Step();
//
//		if (peer.connected)
//		{
//			char buffer[1024];
//			cin.getline(buffer, sizeof(buffer));
//			peer.SendText(buffer);
//		}
//	}
//}

//shared_ptr<Peer> StartPeer(const char *remote_url, int remotePort, int localPort)
//{
//	Peer peer(localPort);
//
//	if (!peer.Start())
//		return nullptr;
//
//	peer.Connect(remote_url, remotePort);
//
//	Loop(peer);
//}
//
KAI_END

#endif
