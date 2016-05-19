#pragma once

KAI_BEGIN

StringStream &operator<<(StringStream &, float);
StringStream &operator>>(StringStream &, float &);
BinaryStream &operator<<(BinaryStream &, float);
BinaryPacket &operator>>(BinaryPacket &, float &);

KAI_TYPE_TRAITS(float, Number::Single , Properties::Arithmetic 
	| Properties::Streaming | Properties::Assign
	| Properties::Relational | Properties::Absolute );

StringStream &operator<<(StringStream &, double);
StringStream &operator>>(StringStream &, double &);
BinaryStream &operator<<(BinaryStream &, double);
BinaryPacket &operator>>(BinaryPacket &, double &);

KAI_TYPE_TRAITS(double, Number::Double
	, Properties::Arithmetic 
	| Properties::Streaming 
	| Properties::Assign
	| Properties::Relational
	| Properties::Absolute
	);

KAI_END


