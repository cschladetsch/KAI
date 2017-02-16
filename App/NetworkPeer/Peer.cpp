#include <iostream>

#include "raknet/BitStream.h"
#include "raknet/RakPeer.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/Kbhit.h"

using namespace std;

struct Peer
{
	const static int InPort = 9999;
	const static int OutPort = 8888;
	const static int MaxConnections = 4;

	typedef unsigned char PacketType;

private:
	RakNet::RakPeerInterface *_peer;
	RakNet::SocketDescriptor _socketDescriptors[2];
	DataStructures::List<RakNet::RakNetSocket2 *> _sockets;

public:
	Peer()
	{
		_peer = RakNet::RakPeerInterface::GetInstance();
		
		_peer->SetTimeoutTime(3000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
		_peer->DisableSecurity();

		// attempt both IPV4 and V6
		_socketDescriptors[0].port = InPort;
		_socketDescriptors[0].socketFamily = AF_INET;
		_socketDescriptors[1].port = InPort;
		_socketDescriptors[1].socketFamily = AF_INET6;
	}

	bool Start()
	{
		int startResult = _peer->Startup(MaxConnections, _socketDescriptors, 1);

		cout << startResult << endl;

		bool started = startResult == RakNet::RAKNET_STARTED;
		
		if (!started)
			return false;

		_peer->SetMaximumIncomingConnections(MaxConnections);
		_peer->SetOccasionalPing(true);
		_peer->SetUnreliableTimeout(1000);

		_peer->GetSockets(_sockets);

		return true;
	}

	bool Connect(const char *ip, int port)
	{
		RakNet::PublicKey *publicKey = 0;
		const char *password = "";
		return _peer->Connect(ip, port, password, strlen(password)) == RakNet::CONNECTION_ATTEMPT_STARTED;
	}

	void Step()
	{
		for (RakNet::Packet *p = _peer->Receive(); p; _peer->DeallocatePacket(p), p = _peer->Receive())
		{
			PacketType type = GetPacketIdentifier(p);

			switch (type)
			{
				case ID_NEW_INCOMING_CONNECTION:
					NewConnection(p);
					break;
			
				default:
					cerr << "Unused packet id " << type << endl;
			}
		}
	}

	void NewConnection(RakNet::Packet *p)
	{
		printf("Connection from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());

		// Record the player ID of the client
		auto clientID = p->systemAddress; 

		printf("Remote internal IDs:\n");
		for (int index=0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
		{
			RakNet::SystemAddress internalId = _peer->GetInternalID(p->systemAddress, index);
			if (internalId != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
			{
				printf("%i. %s\n", index+1, internalId.ToString(true));
			}
		}
	}

	static unsigned char GetPacketIdentifier(RakNet::Packet *p)
	{
		if (p == 0)
			return 255;

		// skip over timestamp to payload
		// TODO: not ignore timestamp?
		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			// const RakNet::Time & = *reinterpret_cast<RakNet::Time *>(p->data);

			return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char) p->data[0];
	}

	void Shutdown()
	{
		_peer->Shutdown(300);
		RakNet::RakPeerInterface::DestroyInstance(_peer);
	}
};

int main(int argc, char **argv)
{
	cout << "Peer" << endl;

	Peer peer;
	if (!peer.Start())
	{
		cerr << "Failed to start peer" << endl;
		return 1;
	}

	while (true)
	{
		peer.Step();

		if (kbhit())
		{
			peer.Connect("127.0.0.1", Peer::InPort);
		}
	}

	return 0;
}
