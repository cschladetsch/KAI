#include <iostream>
#include <map>

#include "KAI/Network/Peer.h"

using namespace std;

KAI_BEGIN

Peer::Peer()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
	
	_peer->SetTimeoutTime(3000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	_peer->DisableSecurity();

}

bool Peer::Start(int listenPort, PacketReceiveCallback r0, ReceiveCallback r1)
{
	// attempt both IPV4 and V6
	_socketDescriptors[0].port = listenPort;
	_socketDescriptors[0].socketFamily = AF_INET;
	_socketDescriptors[1].port = listenPort;
	_socketDescriptors[1].socketFamily = AF_INET6;
	// TODO: try IP6 first. this only attempts IPV4
	// confusingly, the last parameter to Startup
	// is 1-based: so this listens on IP4. changing
	// the 3rd parameter to 2 attempts ip6...
	int startResult = _peer->Startup(MaxConnections, _socketDescriptors, 1);

	bool started = startResult == RakNet::RAKNET_STARTED;
	
	if (!started)
		return false;

	_peer->SetMaximumIncomingConnections(MaxConnections);
	_peer->SetOccasionalPing(true);
	_peer->SetUnreliableTimeout(1000);

	_peer->GetSockets(_sockets);

	return true;
}

bool Peer::Connect(const char *ip, int port)
{
	RakNet::PublicKey *publicKey = 0;
	const char *password = "";
	return _peer->Connect(ip, port, password, strlen(password)) == RakNet::CONNECTION_ATTEMPT_STARTED;
}

void Peer::Step()
{
	for (RakNet::Packet *p = _peer->Receive(); p; _peer->DeallocatePacket(p), p = _peer->Receive())
	{
		PacketType type = GetPacketIdentifier(p);

		//cout << "Type: " << (int)type << endl;

		switch (type)
		{
			case ID_NEW_INCOMING_CONNECTION:
				NewConnection(p);
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION from %s\n", AddressFromPacket(p));
				break;

			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;

			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", AddressFromPacket(p));
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally terminated
				printf("ID_CONNECTION_LOST from %s\n", AddressFromPacket(p));
				break;			

			case GetRemoteListenPort:
				cout << "Got request for remote listen port from " << AddressFromPacket(p);
				break;

			//default:
			//	// just assume it's message data
			//	cout << p->data << endl;
		}
	}
}

// make a dual connection with the sender of packet
void Peer::HandShake(RakNet::Packet *p)
{
	// TODO:
	// Connect(p->systemAddress.ipAddress)
	// RequestPort(...)
	// DisconnectOriginal(...)
	// ReconnectAsRealPeer(...)

	//_peers[++_nextPeerId] = p->systemAddress;

	CompleteConnnection(p);
}

void Peer::NewConnection(RakNet::Packet *p)
{
	printf("Connection from %s with GUID %s\n", AddressFromPacket(p), p->guid.ToString());

	HandShake(p);
}

void Peer::CompleteConnnection(RakNet::Packet *p)
{
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

int Peer::SendText(const char *text)
{
	int messageId = _peer->Send(text, strlen(text) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	if (messageId == 0)
	{
		cerr << "Failed to send " << text << endl;
		return 0;
	}

	return messageId;
}

void Peer::Shutdown()
{
	_peer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

const char *Peer::AddressFromPacket(RakNet::Packet *p)
{
	return p->systemAddress.ToString(true);
}

unsigned char Peer::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == nullptr)
		return 255;

	// skip over timestamp to payload
	// TODO: not ignore timestamp?
	if (static_cast<unsigned char>(p->data[0]) == ID_TIMESTAMP)
	{
		return static_cast<unsigned char>(p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)]);
	}
	else
		return static_cast<unsigned char>(p->data[0]);
}

KAI_END

