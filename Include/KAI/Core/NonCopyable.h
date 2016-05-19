
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_NON_COPYABLE_H
#	define KAI_NON_COPYABLE_H

KAI_BEGIN

/// classes derived from this cannot be copied or constructed from another type
class NonCopyable : public boost::noncopyable
{
};

KAI_END

#endif // KAI_NON_COPYABLE_H

//EOF
