

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BOOST_H
#	define KAI_BOOST_H

// disable locking for shared_ptr
#	define BOOST_SP_DISABLE_THREADS
#	include "boost/shared_ptr.hpp"

#	include "boost/foreach.hpp"
#	define foreach BOOST_FOREACH

#	include "boost/numeric/conversion/cast.hpp"
#	include "boost/utility.hpp"

#	include <boost/unordered_set.hpp>
#	include <boost/unordered_map.hpp>

KAI_BEGIN

using boost::numeric_cast;

namespace numeric
{
   using boost::numeric::bad_numeric_cast;
   using boost::numeric::positive_overflow;
   using boost::numeric::negative_overflow;
}

KAI_END

#endif // KAI_BOOST_H

//EOF
