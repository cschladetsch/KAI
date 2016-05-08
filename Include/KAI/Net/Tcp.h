

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_NETWORK_TCP_H
#	define KAI_NETWORK_TCP_H

KAI_NETWORK_BEGIN

struct TcpClientImpl;
struct TcpServerImpl;
struct UdpClientImpl;

struct NetworkStream;

struct TcpClient
{
private:
	AutoPointer<TcpClientImpl> impl;

public:
	TcpClient();

	void Connect(const String& server, int port);

	NetworkStream &GetStream();
};

struct TcpListener
{
};

struct UdpClient
{
};

KAI_NETWORK_END

#endif // KAI_NETWORK_TCP_H

//EOF
