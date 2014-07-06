

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_NONE_H
#	define KAI_NONE_H

KAI_BEGIN

struct None
{
	typedef None Prev, Next, Value, Type;
};

//StringStream &operator<<(StringStream &S, None);
//StringStream &operator<<(StringStream &S, None &);

KAI_END

#endif // KAI_NONE_H

//EOF
