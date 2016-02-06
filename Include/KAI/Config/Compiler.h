
#ifndef KAI_CONFIG_COMPILER_H
#	define KAI_CONFIG_COMPILER_H
#
#	if defined(BOOST_MSVC)
#		define KAI_COMPILER_MSVC
#	endif
#
#	undef KAI_HAVE_PRAGMA_ONCE

#pragma warning (disable: 4458 4456)


#
#	ifdef KAI_COMPILER_MSVC
#		ifndef KAI_HAVE_PRAGMA_ONCE
#			define KAI_HAVE_PRAGMA_ONCE
#		endif

#	endif // KAI_COMPILER_MSVC
#endif // KAI_CONFIG_COMPILER_H

//EOF
