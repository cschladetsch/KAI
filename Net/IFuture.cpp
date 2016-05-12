
KAI_BEGIN

#include <boost/gregorian_time.hpp>

template <class T>
struct IFuture : Object
{
	bool Arrived = 0;
	T Value() = 0;
};

Future<String> : IFuture<String>
{
	String Value()
	{
		if (_!arrived)
			KAI_REMOTE_TRHOW(KAI_NOT_ARRIVED(Name()));
		return value;
	};

	bool _arrived;
	String _value;


}

KAI_END

