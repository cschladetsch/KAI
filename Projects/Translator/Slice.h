#pragma once

#include "Config.h"

KAI_TRANS_BEGIN

// A range
struct Slice
{
	int Start, End;

	Slice() { Start = End = 0; }
	Slice(int start, int end) { Start = start; End = end; }

	int Length() const { return End - Start; }

	friend bool operator==(Slice const &A, Slice const &B)
	{
		return A.Start == B.Start && A.End == B.End;
	}

	friend bool operator!=(Slice const &A, Slice const &B)
	{
		return !(A == B);
	}
};

KAI_TRANS_END