#pragma once

#include <string>

KAI_BEGIN

class String
{
public:
	typedef char Char;
	//typedef typename std::basic_string<Char> Storage;
	typedef std::string Storage;
	typedef typename Storage::const_iterator const_iterator;
	typedef typename Storage::iterator iterator;

private:
	Storage _myString;

public:
	String() { }

	//  TODO STRING WTF
	//template <class II>
	//String(II A, II B) { _string.assign(A,B); }

	String(const Char *S) { if (S != 0) _myString = S; }

	String(const String &X) { _myString = X._myString; }

	String(int N, Char C) { _myString = std::move(std::string(N, C)); }

	int foo()
	{
		return 0;
	}

	//String &operator+=(const String &A)
	//{
	//	_string += A._string;
	//	return *this;
	//}

	//friend String &operator+=(Char A)
	//{
	//	_string += A;
	//	return *this;
	//}

	//String operator+(const String &A) { return A._string + _string; }

	const_iterator begin() const { return _myString.begin(); }
	const_iterator end() const { return _myString.end(); }

	iterator begin() { return _myString.begin(); }
	iterator end() { return _myString.end(); }

	// TODO: where did std::erase go?
	//iterator erase(iterator A, iterator B) { return _string.erase(A,B); }

	// TODO WTF
	//template <class II0, class II1>
	//void insert(II0 where, II1 begin, II1 end) { _myString.insert(where, begin, end); }
	
	int size() const { return (int)_myString.size(); }

	bool empty() const { return _myString.empty(); }

	std::string StdString() const { return _myString; }
	const Storage &GetStorage() const { return _myString; }
	void clear() { _myString.clear(); }
	int Size() const { return size(); }
	bool Empty() const { return empty() || _myString[0] == 0; }
	const_iterator Begin() const { return begin(); }
	void Clear() { _myString.clear(); }

	String tolower() const { return LowerCase(); }
	String toupper() { return UpperCase(); }

	String LowerCase()const ;
	String UpperCase()const ;
	String Capitalise()const ;
	
	bool Contains(String const &) const;
	bool StartsWith(String const &) const;
	bool EndsWith(String const &) const;

	Char &operator[](int N) { return _myString.at(N); }
	const Char &operator[](int N) const { return _myString.at(N); }
	const Char *c_str() const { return _myString.c_str(); }

	void ReplaceFirst(String const &what, String const &with);
	void ReplaceLast(String const &what, String const &with);
	void RemoveAll(String const &what);

	static void Register(Registry &);
};

bool operator<(const String &A, const String &B);
bool operator==(const String &A, const String &B);
bool operator>(const String &A, const String &B);

StringStream &operator<<(StringStream &, const String &);
StringStream &operator>>(StringStream &, String &);
BinaryStream &operator<<(BinaryStream &, const String &);
BinaryPacket &operator>>(BinaryPacket &, String &);

//StringStream &operator<<(StringStream &, const char *);

//StringStream &operator<<(StringStream &K, std::string const &S) { return K << S.c_str(); }

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
