#pragma once

#include "KAI/Base.h"
#include "RakNet/RakPeer.h"
#include "RakNet/MessageIdentifiers.h"

#include <map>

KAI_BEGIN

// basic connection between two processes over TCP
struct Peer
{
	enum MessageId
	{
		GetRemoteListenPort = ID_USER_PACKET_ENUM + 1,
	};

	// TODO
	bool connected = false;
	bool isServer = false;

	const static int MaxConnections = 255;

	typedef unsigned char PacketType;

	typedef std::map<int, RakNet::SystemAddress> Peers;

private:
	RakNet::RakPeerInterface *_peer;
	RakNet::SocketDescriptor _socketDescriptors[2];
	DataStructures::List<RakNet::RakNetSocket2 *> _sockets;
	Peers _peers;
	int _nextPeerId;

public:
	Peer(int listenPort);

	bool Start();
	bool Connect(const char *ip, int port);
	void Step();
	int SendText(const char *text);

	static const char *AddressFromPacket(RakNet::Packet *p);

private:
	// make a dual connection with the sender of packet
	void HandShake(RakNet::Packet *p);
	void NewConnection(RakNet::Packet *p);

	void CompleteConnnection(RakNet::Packet *p);
	static unsigned char GetPacketIdentifier(RakNet::Packet *p);
	void Shutdown();
};

KAI_END

