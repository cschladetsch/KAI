#pragma once

KAI_TYPE_BEGIN

enum Properties;

inline Properties operator|(Properties a, Properties b)
{
	return static_cast<Properties>(static_cast<int>(a) | static_cast<int>(b));
}

/// Describes the properties of a promoted type
enum Properties : int
{
	Nothing = 0,
	Assign = 1 << 0,
	Reflected = 1 << 1,

	Plus = 1 << 2,
	Minus = 1 << 3,

	Less = 1 << 6,
	Equiv = 1 << 7,
	Greater = 1 << 8,
	
	StringStreamInsert = 1 << 9,
	StringStreamExtract = 1 << 10,
	BinaryStreamInsert = 1 << 11,
	BinaryStreamExtract = 1 << 12,

	NoHashValue = 1 << 13,
	CalcHashValue = 1 << 14,

	Container = 1 << 15,
	Process = 1 << 16,
	Absolute = 1 << 17,

	Divide = 1 << 18,
	XmlOutput = 1 << 20,
};

KAI_TYPE_END

