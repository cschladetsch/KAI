// auto-generated on Tuesday; modify at own risk
#include <KAI/Network/ProxyDecl.h>

namespace MLB0
{
	struct Test0Proxy: ProxyBase
	{
		using ProxyBase::StreamType;
		Test0Proxy(Node &node, NetHandle handle) : ProxyBase(node, handle) { }
		
		Future<int> num()
		{
			return Fetch<int>("num");
		}
		Future<void> fun0()
		{
			return Exec<void>("fun0");
			
		}
		Future<void> fun1(int n)
		{
			StreamType args;
			args << n;
			return Exec<void>("fun1", args);
			
		}
		Future<int> fun1r(int n)
		{
			StreamType args;
			args << n;
			return Exec<int>("fun1r", args);
			
		}
		
	}
}
