#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CONFIG_INTEGRAL_TYPES_H
#	define KAI_CONFIG_INTEGRAL_TYPES_H

#include <boost/integer.hpp>

KAI_BEGIN

typedef boost::int_t<8> int8;
typedef boost::int_t<16> int16;
typedef boost::int_t<32> int32;
typedef boost::int_t<64> int64;

typedef boost::uint_t<8> uint8;
typedef boost::uint_t<16> uint16;
typedef boost::uint_t<32> uint32;
typedef boost::uint_t<64> uint64;

KAI_END

#endif // KAI_CONFIG_INTEGRAL_TYPES_H

// EOF
