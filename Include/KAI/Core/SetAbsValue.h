#pragma once

KAI_BEGIN

template <struct Ref>
struct SetAbsValue
{
	void Perform(Ref r)
	{
		if (r < 0)
			r = -r;
	}
};

KAI_END

