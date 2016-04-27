#pragma once

KAI_BEGIN

/// A Reflected type has a Self pointer. This is semantically same as C++'s 'this' pointer,
/// but using the Object Model of the system
class Reflected : public ReflectedBase
{
public:
	Registry &Reg() const
	{
		return *Self->GetRegistry();
	}
};

KAI_END
