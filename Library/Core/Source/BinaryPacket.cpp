#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Object/ClassBuilder.h"

KAI_BEGIN

void BinaryPacket::Register(Registry &registry, const char *name)
{
	ClassBuilder<BinaryPacket>(registry, Label(name))
		.Methods
		(Label("Size"), &BinaryPacket::Size)
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

void BinaryStream::Register(Registry &registry)
{
	ClassBuilder<BinaryStream>(registry, Label(Type::Traits<BinaryStream>::Name()))
		.Methods
			(Label("Size"), &BinaryStream::Size)
			(Label("Clear"), &BinaryStream::Clear)
		;
}

void BinaryStream::Clear()
{
	bytes.clear();
	first = current = last = 0;
}

bool BinaryStream::Write(int len, const Byte *src)
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

	return true;
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

BinaryPacket &operator>>(BinaryPacket &S, int &N)
{
	if (!S.Read(N))
		KAI_THROW_0(PacketExtraction);

	return S;
}

BinaryPacket &operator>>(BinaryPacket &S, double &N)
{
	if (!S.Read(N))
		KAI_THROW_0(PacketExtraction);

	return S;
}

BinaryPacket &operator>>(BinaryPacket &S, float &N)
{
	if (!S.Read(N))
		KAI_THROW_0(PacketExtraction);

	return S;
}

BinaryPacket &operator>>(BinaryPacket &S, bool &N)
{
	if (!S.Read(N))
		KAI_THROW_0(PacketExtraction);

	return S;
}

BinaryStream &operator<<(BinaryStream &S, const BinaryPacket &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, const BinaryStream &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, double N)
{
	if (!S.Write(N))
		KAI_THROW_0(PacketInsertion);

	return S;
}

BinaryStream &operator<<(BinaryStream &S, float N)
{
	if (!S.Write(N))
		KAI_THROW_0(PacketInsertion);

	return S;
}

BinaryStream &operator<<(BinaryStream &S, bool N)
{
	if (!S.Write(N))
		KAI_THROW_0(PacketInsertion);

	return S;
}

BinaryStream &operator<<(BinaryStream &S, int N)
{
	if (!S.Write(N))
		KAI_THROW_0(PacketInsertion);

	return S;
}

KAI_END

//EOF
