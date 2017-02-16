#pragma once

#include "raknet/RakPeer.h"
#include "raknet/MessageIdentifiers.h"

#include <map>

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

	static const char *AddressFromPacket(RakNet::Packet *p);

	void Step();
	// make a dual connection with the sender of packet
	void HandShake(RakNet::Packet *p);
	void NewConnection(RakNet::Packet *p);

	void CompleteConnnection(RakNet::Packet *p);
	static unsigned char GetPacketIdentifier(RakNet::Packet *p);
	int SendText(const char *text);
	void Shutdown();
};

