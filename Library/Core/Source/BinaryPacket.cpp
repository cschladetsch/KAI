#include "KAI/Core/BuiltinTypes.h"

KAI_BEGIN

void BinaryPacket::Register(Registry &registry, const char *name)
{
	ClassBuilder<BinaryPacket>(registry, Label(name))
		.Methods
			("Size", &BinaryPacket::Size)
		;
}

bool BinaryPacket::Read(int len, Byte *dest)
{
	if (!CanRead(len))
		return false;

	memcpy(dest, current, len);
	current += len;
	return true;
}

bool BinaryPacket::CanRead(int len) const
{
	return len > 0 && current + len <= last;
}

void BinaryStream::Clear()
{
	bytes.clear();
	first = current = last = 0;
}

BinaryStream &BinaryStream::Write(int len, const Byte *src)
{
	// TODO: this is just temporary; production will of course avoid re-allocation on every write!
	std::size_t cursor = current - first;
	std::size_t cur_size = last - first;
	bytes.resize(bytes.size() + len);
	first = &*bytes.begin();
	current = first + cursor;
	last = &*--bytes.end();
	last++;

	memcpy((void *)(first + cur_size), src, len);

	return *this;
}

BinaryPacket &operator>>(BinaryPacket &S, BinaryPacket &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

BinaryPacket &operator>>(BinaryPacket &S, BinaryStream &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

KAI_END

//EOF
