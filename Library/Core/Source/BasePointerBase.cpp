#include <KAI/Core/Config/Base.h>
#include <KAI/Core/BasePointer.h>
#include <KAI/Core/Exception/ExceptionMacros.h>
#include <KAI/Core/Exception/Extended.h>

KAI_BEGIN

StringStream &operator<<(StringStream &S, BasePointerBase const &)
{
	KAI_NOT_IMPLEMENTED();
}

KAI_END

