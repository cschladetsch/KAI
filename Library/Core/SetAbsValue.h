#pragma once

#include <KAI/Core/Config/Base.h>

KAI_BEGIN

	template <class T>
	struct SetAbsValue
	{
		void Perform(Reference & T)
		{
			if (T < 0)
				T = -1;
		}

	};


KAI_END

