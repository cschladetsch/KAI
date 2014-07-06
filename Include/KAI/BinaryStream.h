

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BINARY_STREAM_H
#	define KAI_BINARY_STREAM_H

KAI_BEGIN

/// A Binary-packet is a fixed-size sequence of bytes which allows only extraction
struct BinaryPacket
{
	typedef char Byte;
	typedef const Byte *const_iterator;

protected:
	const_iterator first, current, last;
	Registry *registry;

public:
	BinaryPacket() : registry(0) { first = last = current = 0; }
	BinaryPacket(Registry &R) : registry(&R) { first = last = current = 0; }
	BinaryPacket(const_iterator F, const_iterator L, Registry *R = 0) : first(F), last(L), current(0), registry(R) { }

	const_iterator Begin() const { return first; }
	const_iterator Current() const { return current; }
	const_iterator Last() const { return last; }

	int Size() const { return (int)(last - first); }
	bool Empty() const { return last == first; }
	bool Read(int len, Byte *dest);
	bool CanRead(int len) const;

	template <class POD>
	bool Read(POD &pod)
	{
		return Read(sizeof(pod), reinterpret_cast<Byte *>(&pod));
	}

	void SetRegistry(Registry *R) { registry = R; }
	Registry *GetRegistry() const { return registry; }

	static void Register(Registry &, const char *);

	friend bool operator<(const BinaryPacket &A, const BinaryPacket &B);
	friend bool operator==(const BinaryPacket &A, const BinaryPacket &B);
};

StringStream &operator<<(StringStream &, BinaryPacket const &);
BinaryStream &operator<<(BinaryStream &, BinaryPacket const &);
BinaryPacket &operator>>(BinaryPacket &, BinaryPacket &);

KAI_TYPE_TRAITS(BinaryPacket, Type::Number::BinaryPacket
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreamExtract 
	| Type::Properties::BinaryStreamInsert);

/// A BinaryStream is-a BinaryPacket which can also resize and allows insertion
struct BinaryStream : BinaryPacket
{
	typedef nstd::vector<Byte> Bytes;

private:
	Bytes bytes;

public:
	BinaryStream() { }
	BinaryStream(Registry &R) : BinaryPacket(R) { }
	//BinaryStream(int start_size) : bytes(start_size) { }

	bool Write(int len, const Byte *src);
	void Clear();

	template <class POD>
	bool Write(const POD &pod)
	{
		return Write(sizeof(pod), reinterpret_cast<const Byte *>(&pod));
	}

	friend bool operator<(const BinaryPacket &A, const BinaryPacket &B);
	friend bool operator==(const BinaryPacket &A, const BinaryPacket &B);

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, BinaryStream const &);
BinaryStream &operator<<(BinaryStream &, BinaryStream const &);
BinaryPacket &operator>>(BinaryPacket &, BinaryStream &);

KAI_TYPE_TRAITS(BinaryStream, Type::Number::BinaryStream
	, Type::Properties::StringStreamInsert | Type::Properties::BinaryStreamExtract | Type::Properties::BinaryStreamInsert);

KAI_END

#endif // KAI_BINARY_STREAM_H

//EOF

