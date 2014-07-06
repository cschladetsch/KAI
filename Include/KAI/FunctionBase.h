
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_FUNCTION_BASE_H
#	define KAI_FUNCTION_BASE_H

KAI_BEGIN

struct FunctionBase : CallableBase<FunctionBase>
{
	FunctionBase(const Label &L) : CallableBase<FunctionBase>(L) { }

	virtual void Invoke(Registry &, Stack &stack) = 0;
	String ToString() const;
	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, FunctionBase const &);
StringStream &operator<<(StringStream &, const BasePointer<FunctionBase> &);

KAI_TYPE_TRAITS(BasePointer<FunctionBase>, Type::Number::Function, Type::Properties::StringStreamInsert | Type::Properties::Reflected);

KAI_END

#endif // KAI_FUNCTION_BASE_H

//EOF

