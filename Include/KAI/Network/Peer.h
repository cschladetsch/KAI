#pragma once

#include <map>
#include <functional>

#include <RakNet/RakPeer.h>
#include <RakNet/MessageIdentifiers.h>

#include "KAI/Base.h"

KAI_BEGIN

struct Peer;

KAI_TYPE_TRAITS(
    Peer, 
    1000, 
    Properties::StringStreamInsert
);

KAI_TYPE_TRAITS(
    RakNet::Packet, 
    1001,
    Properties::StringStreamInsert
);

// basic connection between two processes over UDP and/or TCP
struct Peer : Reflected
{
    enum MessageId
    {
        GetRemoteListenPort = ID_USER_PACKET_ENUM + 1,
    };

    // TODO
    Pointer<bool> connected;
    Pointer<bool> isServer;

    const static int MaxConnections = 255;

private:
    typedef unsigned char PacketType;
    typedef std::map<int, RakNet::SystemAddress> Peers;
	
    RakNet::RakPeerInterface *_peer;
    DataStructures::List<RakNet::RakNetSocket2 *> _sockets;
    RakNet::SocketDescriptor _socketDescriptors[2];
    Peers _peers;
    int _nextPeerId;

public:
    Peer();
    virtual ~Peer() = default;

    typedef std::function<void(Pointer<Peer>, Pointer<String>)> ReceiveCallback;
    typedef std::function<void(Pointer<Peer>, Pointer<RakNet::Packet>)> PacketReceiveCallback;

    bool Start(int listenport, PacketReceiveCallback = &Peer::DefaultPacketReceiveCallback, ReceiveCallback = &Peer::DefaultReceiveCallback);
    bool Connect(const char *ip, int port);
    int SendText(const char *text);
    int SendText2(Pointer<String>);
    int SendBlob(Pointer<BinaryPacket>);
    void Step();

    static const char *AddressFromPacket(RakNet::Packet *p);

    static void Register(Registry &);

private:
    // make a dual connection with the sender of packet
    void HandShake(RakNet::Packet *p);
    void NewConnection(RakNet::Packet *p);

    void CompleteConnnection(RakNet::Packet *p);
    static unsigned char GetPacketIdentifier(RakNet::Packet *p);
    void Shutdown();

    static void DefaultPacketReceiveCallback(Pointer<Peer>, Pointer<RakNet::Packet>);
    static void DefaultReceiveCallback(Pointer<Peer>, Pointer<String>);
};

StringStream &operator<<(StringStream &, Peer const &);
StringStream &operator<<(StringStream &, RakNet::Packet const &);

KAI_END

