#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_STRING_H
#	define KAI_STRING_H

KAI_BEGIN

struct String
{
	typedef char Char;
	typedef std::basic_string<Char> Storage;
	//typedef nstd::string Storage;
	typedef Storage::const_iterator const_iterator;
	typedef Storage::iterator iterator;

private:
	Storage string;

public:
	String() { }
	template <class II>
	String(II A, II B) { string.assign(A,B); }// : string(A, B) { }
	String(const Char *S) { if (S != 0) string = S; }
	String(const Storage &S) : string(S) { }
	String(const String &X) : string(X.string) { }
	String(int N, Char C) : string(N, C) { }

	String &operator+=(const String &A)
	{
		string += A.string;
		return *this;
	}

	String &operator+=(Char A)
	{
		string += A;
		return *this;
	}

	friend String operator+(const String &A, const String &B) { return A.string + B.string; }
	friend bool operator<(const String &A, const String &B);
	friend bool operator==(const String &A, const String &B);
	friend bool operator>(const String &A, const String &B);

	const_iterator begin() const { return string.begin(); }
	const_iterator end() const { return string.end(); }
	iterator begin() { return string.begin(); }
	iterator end() { return string.end(); }
	iterator erase(iterator A, iterator B) { return string.erase(A,B); }
	template <class II0, class II1>
	void insert(II0 where, II1 begin, II1 end) { string.insert(where, begin, end); }
	int size() const { return (int)string.size(); }
	bool empty() const { return string.empty(); }
	nstd::string StdString() const { return string; }
	const Storage &GetStorage() const { return string; }
	void clear() { string.clear(); }
	int Size() const { return size(); }
	bool Empty() const { return empty() || string[0] == 0; }
	const_iterator Begin() const { return begin(); }
	void Clear() { string.clear(); }

	String tolower() const { return LowerCase(); }
	String toupper() { return UpperCase(); }

	String LowerCase()const ;
	String UpperCase()const ;
	String Capitalise()const ;
	
	bool Contains(String const &) const;
	bool StartsWith(String const &) const;
	bool EndsWith(String const &) const;

	Char &operator[](int N) { return string.at(N); }
	const Char &operator[](int N) const { return string.at(N); }
	const Char *c_str() const { return string.c_str(); }

	void ReplaceFirst(String const &what, String const &with);
	void ReplaceLast(String const &what, String const &with);
	void RemoveAll(String const &what);

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const String &);
StringStream &operator>>(StringStream &, String &);
BinaryStream &operator<<(BinaryStream &, const String &);
BinaryPacket &operator>>(BinaryPacket &, String &);

StringStream &operator<<(StringStream &, const char *);

inline StringStream &operator<<(StringStream &K, nstd::string const &S) { return K << S.c_str(); }

KAI_END

namespace boost
{
	#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
	  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
	#define get16bits(d) (*((const unsigned short *) (d)))
	#endif

	#if !defined (get16bits)
	#define get16bits(d) ((((size_t)(((const unsigned char *)(d))[1])) << 8)\
						   +(size_t)(((const unsigned char *)(d))[0]) )
	#endif
	inline size_t hash_value(KAI_NAMESPACE(String) const &string)
	{
		size_t len = string.size();
		size_t hash = len, tmp;
		if (len <= 0)
			return 0;
		const char *data = reinterpret_cast<const char *>(&*string.begin());
		size_t rem = len & 3;
		len >>= 2;
		// Main loop; 4 bytes each iteration
		for (;len > 0; len--) 
		{
			hash  += get16bits (data);
			tmp    = (get16bits (data+2) << 11) ^ hash;
			hash   = (hash << 16) ^ tmp;
			data  += 4;
			hash  += hash >> 11;
		}
		// handle end cases
		switch (rem) 
		{
		case 3: hash += get16bits (data);
				hash ^= hash << 16;
				hash ^= data[sizeof (unsigned short)] << 18;
				hash += hash >> 11;
				break;
		case 2: hash += get16bits (data);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
		case 1: hash += *data;
				hash ^= hash << 10;
				hash += hash >> 1;
		}

		// Force "avalanching" of final 127 bits
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;
		return hash;
	}
	#undef get16bits
}

#endif // KAI_STRING_H

//EOF
