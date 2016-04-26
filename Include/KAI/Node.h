#pragma once

KAI_BEGIN

struct NodeInterface
{
	virtual void Method0() = 0;
	virtual void Method1(String, int) = 0;
	virtual String Method2() = 0;

	typedef MethodList< 
		&NodeInterface::Method0
		, &NodeInterface::Method1
		, &NodeInterface::Method2
		> Methods;
};

template <class Iface>
struct Proxy
{
	BinaryStream stream;
	MarshallerBase Invoke(int number)
	{
		switch (number)
		{
		case 0: return Marshaller<Iface::Method<0> >::Type(stream);
		case 1: return Marshaller<Iface::Method<1> >::Type(stream);
		}
	}
};

template <class Iface>
struct Servant
{
	Iface *servant;
	BinaryPacket *packet;
	void Serve()
	{
		if (!packet)
			return;
		int number;
		*packet >> number;
		switch (number)
		{
		case 0: Service<Iface::Method<0> >::Type(servant, *packet);
		case 1: Invoker<Iface::Method<1> >::Type(servant, *packet);
		}
	}
};

struct Usage
{
	Proxy<NodeInterface> node_proxy;
	Server<NodeInterface> node_server;

	node_server.SetStreamSource(node_proxy.GetStreamSource());

	node_proxy.Request<&Iface::Method0>::Given(42, "asd");

	node_server.Serve();
};

struct NodeProxy : Implements<NodeInterface>
{
	static void Bind(Node &node)
	{
	}
};

KAI_END
