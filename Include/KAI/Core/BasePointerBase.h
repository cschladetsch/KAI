#pragma once

KAI_BEGIN

struct BasePointerBase
{
	void Create() {}
	bool Destroy() { return true; }

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const BasePointerBase &);
StringStream &operator<<(StringStream &, const BasePointerBase &);
StringStream &operator<<(StringStream &, const BasePointerBase &);
StringStream &operator<<(StringStream &, const BasePointerBase &);
StringStream &operator<<(StringStream &, const BasePointerBase &);

KAI_END

