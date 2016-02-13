
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_STD_CONTAINERS_H
#	define KAI_STD_CONTAINERS_H

#	include "KAI/Config/Options.h"
#	include <string>
#	include <map>
#	include <unordered_map>
#	include <set>
#	include <unordered_set>
#	include <vector>
#	include <list>
#	include <algorithm>

KAI_BEGIN

namespace nstd
{
	using std::size_t;
	using std::pair;
	using std::find;
	using std::make_pair;
	using std::auto_ptr;

	using std::unordered_map;
	using std::unordered_set;

	using std::string;
	using std::vector;
	using std::list;
	using std::map;
	using std::set;
};

KAI_END

#endif // KAI_STD_CONTAINERS_H

//EOF
