#pragma once

#include "KAI/Pathname.h"

KAI_BEGIN

namespace Exception
{
	struct CannotResolve : Base
	{
		Object object;
		Label label;
		Pathname path;
		CannotResolve(const FileLocation &L, Object const &Q) : Base(L, "CannotResolve"), object(Q) { }
		CannotResolve(const FileLocation &L, Label const &Q) : Base(L, "CannotResolve"), label(Q) { }
		CannotResolve(const FileLocation &L, Pathname const &Q) : Base(L, "CannotResolve"), path(Q) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct InvalidPathname : Base
	{
		String text;
		InvalidPathname(const FileLocation &L, const String &T) : Base(L, "InvalidPathname"), text(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct InvalidIdentifier : Base
	{
		Object what;
		InvalidIdentifier(const FileLocation &L, const Object &T) : Base(L, "InvalidIdentifier"), what(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct NotImplemented : Base
	{
		String text;
		NotImplemented(const FileLocation &L, const char *T = "<unnamed>") : Base(L, "Not Implemented"), text(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct ObjectNotInTree : Base
	{
		Object object;
		ObjectNotInTree(const FileLocation &L, Object const &Q) : Base(L, "Object not in tree"), object(Q) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct InternalError : Base
	{
		InternalError(const FileLocation &L, const char *T) : Base(L, T) { }
	};
	struct UnknownMethod : Base
	{
		String name;
		String class_name;
		UnknownMethod(const FileLocation &L, const String &N, const String &B) : Base(L, "Unknown Method"), name(N), class_name(B) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct UnknownHandle : Base
	{
		Handle handle;
		UnknownHandle(const FileLocation &L, Handle N) : Base(L, "Unknown Handle"), handle(N) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct InvalidStringLiteral : Base
	{
		String text;
		InvalidStringLiteral(const FileLocation &L, const String &T) : Base(L, "Invalid string literal"), text(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	template <class T = meta::Null>
	struct UnknownClass : Base
	{
		String name;
		Type::Number type_number;
		UnknownClass(const FileLocation &L, const String &N) : Base(L, "Unknown Class"), name(N) { }
		UnknownClass(const FileLocation &L, Type::Number N) : Base(L, "Unknown Class"), type_number(N) { }
		UnknownClass(const FileLocation &L) : Base(L, "Unknown Class"), type_number(Type::Traits<T>::Number), name(Type::Traits<T>::Name) { }
		void WriteExtendedInformation(StringStream &S) const
		{
			S << "name=" << name << ", type_number=" << type_number.ToInt();
		}
	};
	struct CannotNew : Base
	{
		Object arg;
		CannotNew(const FileLocation &L, const Object &Q) : Base(L, "Cannot new"), arg(Q) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct AssertionFailed : Base
	{
		AssertionFailed(const FileLocation &L) : Base(L, "AssertionFailed") { }
	};
	struct UnknownKey : Base
	{
		Object key;
		UnknownKey(const FileLocation &L, Object K) : Base(L, "Unknown key in Map"), key(K) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct BadIndex : Base
	{
		int index;
		BadIndex(const FileLocation &L, int N) : Base(L, "Bad Index"), index(N) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct BadUpCast : Base
	{
		BadUpCast(const FileLocation &L) : Base(L, "BadUpCast") { }
	};
	struct FileNotFound : Base
	{
		String filename;
		FileNotFound(const FileLocation &L, String const &F) : Base(L, "FileNotFound"), filename(F) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	struct UnknownProperty : Base
	{
		Label klass;
		Label prop;
		UnknownProperty(const FileLocation &L, Label const &K, Label const &P) : Base(L, "UnknownProperty"), klass(K), prop(P) { }
		void WriteExtendedInformation(StringStream &) const;
	};
	
}

KAI_END // KAI_EXCEPTION_EXTENDED_H
