#pragma once

#include <string>
#include <KAI/Core/Exception/FileLocation.h>
#include <KAI/Core/Object/Handle.h>
#include <KAI/Core/StringStream.h>

KAI_BEGIN

namespace Exception
{
	struct Base : std::exception
	{
		FileLocation location;
		std::string text;

		Base() { }
		Base(const char * const msg) : std::exception(msg), text(msg) { }
		Base(const FileLocation &L, const char *T = "Exception")
			: std::exception(T), location(L), text(T) { }

		std::string ToString() const;
		virtual void WriteExtendedInformation(StringStream &) const { }
	};

	struct LogicError : Base
	{
		LogicError(const FileLocation &loc, const char *text)
			: Base(loc, text) { }
	};

	struct TypeMismatch : Base
	{
		int first, second;
		TypeMismatch(const FileLocation &L , int A = 0, int B = 0 , const char *T = "Type Mismatch")
			: Base(L, T), first(A), second(B) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct NullObject : Base
	{
		NullObject(const FileLocation &L, const char *T = "Null Object")
			: Base(L, T) { }
	};

	struct EmptyStack : Base
	{
		EmptyStack(const FileLocation &L, const char *T = "Empty Stack")
			: Base(L, T) { }
	};
	struct ConstError : Base
	{
		ConstError(const FileLocation &L, const char *T = "Const Error")
			: Base(L, T) { }
	};
	struct UnknownTypeNumber : Base
	{
		int type_number;
		UnknownTypeNumber(const FileLocation &L, int N = 0, const char *T = "Unknown Type Number")
			: Base(L, T), type_number(N) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct UnknownObject : Base
	{
		Handle handle;
		UnknownObject(const FileLocation &L, Handle H)
			: Base(L, "Unknown Object"), handle(H) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct ObjectNotFound : Base
	{
		String label;
		ObjectNotFound(const FileLocation &L, const String &A = "")
			: Base(L, "ObjectNotFound"), label(A) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct Assertion : Base
	{
		Assertion(const FileLocation &L)
			: Base(L, "AssertionFailed") { }
	};

	struct NoOperation : Base
	{
		int type_property;
		int type_number;
		NoOperation(const FileLocation &L)
			: Base(L, "Class does not have required operation"), type_property(0), type_number(0) { }
		NoOperation(const FileLocation &L, int N, int P)
			: Base(L, "Class does not have required opeation"), type_number(N), type_property(P) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct PacketExtraction : Base
	{
		int type_number;
		PacketExtraction(const FileLocation &L, int T = 0)
			: Base(L, "Cannot extract"), type_number(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct PacketInsertion : Base
	{
		int type_number;
		PacketInsertion(const FileLocation &L, int T = 0)
			: Base(L, "Cannot insert"), type_number(T) { }
		void WriteExtendedInformation(StringStream &) const;
	};

	struct OutOfBounds : Base
	{
		int type_number;
		int index;

		OutOfBounds(const FileLocation &L, int idx, int ty)
			: Base(L, "Out of Bounds"), type_number(ty), index(idx) { }
		void WriteExtendedInformation(StringStream &) const;
	};
}

StringStream &operator<<(StringStream &, Exception::Base const &);

KAI_END

