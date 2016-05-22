#pragma once

#include <KAI/Network/Node.h>
#include <KAI/Network/Proxy.h>
#include <KAI/Network/Agent.h>

KAI_NET_BEGIN

class Domain
{
public:

	Domain(Node &);

	template <class T>
	Agent<T> MakeAgent();

	template <class T>
	Proxy<T> MakeProxy(NetHandle handle);

private:
	Node &_node;
};

KAI_NET_END
